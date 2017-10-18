/*
 * Compile this file together with the jx9 scripting engine source code to generate
 * the executable. For example: 
 *  gcc -W -Wall -O6 -o test jx9_hostapp_info.c jx9.c
*/
/*
 * This simple program is a quick introduction on how to embed and start
 * experimenting with the JX9 scripting engine without having to do a lot
 * of tedious reading and configuration.
 *
 * For an introduction to the JX9 C/C++ interface, please refer to this page
 *        http://jx9.symisc.net/api_intro.html
 * For the full C/C++ API reference guide, please refer to this page
 *        http://jx9.symisc.net/c_api.html
 * For the full list of built-in functions (over 303), please refer to this page
 *        http://jx9.symisc.net/builtin_func.html
 * For the full JX9 language reference manual, please refer to this page
 *        http://jx9.symisc.net/jx9_lang.html
 */
/*
 * The following program demonstrate how data is shared between the host application
 * and the running JX9 script. The main() function defined below creates and install
 * two foreign variables named respectively $my_app and $my_data. The first is a simple
 * scalar value while the last is a complex JSON object. these foreign variables are
 * made available to the running script using the [jx9_vm_config()] interface with
 * a configuration verb set to JX9_VM_CONFIG_CREATE_VAR.
 * 
 * JX9 Program:
 *
 * print "Showing foreign variable contents\n";
 * //Scalar foreign variable named $my_app
 * print "\$my_app =",$my_app..JX9_EOL;
 * //Foreign JSON object named $my_data
 * print "\$my_data = ",$my_data;
 * //Dump command line arguments
 * if( count($argv) > 0 ){
 *  print "\nCommand line arguments:\n";
 *  print $argv;
 * }else{
 *  print "\nEmpty command line";
 * }
 * //Return a simple JSON object to the host application
 * $my_config = {
 *        "jx9_version" : __JX9__,  //JX9 Engine version
 *        "time" : __TIME__, //Current time
 *        "date" : __DATE__  //Current date
 * };
 * //terminate the script
 * die;
 */
#define JX9_PROG \
 "print \"Showing foreign variable contents\n\n\";"\
 " /*Scalar foreign variable named $my_app*/"\
 " print \"\\$my_app = \",$my_app..JX9_EOL;"\
 " /*JSON object foreign variable named $my_data*/"\
 " print \"\n\\$my_data = \",$my_data..JX9_EOL;"\
 " /*Dump command line arguments*/"\
 " if( count($argv) > 0 ){"\
 "  print \"\nCommand line arguments:\n\";"\
 "  print $argv..JX9_EOL;"\
 " }else{"\
 "  print \"\nEmpty command line\";"\
 " }"\
 " /*Return a simple JSON object to the host application*/"\
 " $my_config = {"\
 "        'jx9_version' : __JX9__,  /*JX9 Engine version*/"\
 "        'time' : __TIME__, /*Current time*/"\
 "        'date' : __DATE__  /*Current date*/"\
 " };"\
 "/*terminate the script*/"\
 " die;"

/* Make sure you have the latest release of the JX9 engine
 * from:
 *  http://jx9.symisc.net/downloads.html
 */
#include <stdio.h>
#include <stdlib.h>
/* Make sure this header file is available.*/
#include "jx9.h"
/* 
 * Display an error message and exit.
 */
static void Fatal(const char *zMsg)
{
	puts(zMsg);
	/* Shutdown the library */
	jx9_lib_shutdown();
	/* Exit immediately */
	exit(0);
}
/*
 * VM output consumer callback.
 * Each time the virtual machine generates some outputs, the following
 * function gets called by the underlying virtual machine to consume
 * the generated output.
 * All this function does is redirecting the VM output to STDOUT.
 * This function is registered later via a call to jx9_vm_config()
 * with a configuration verb set to: JX9_VM_CONFIG_OUTPUT.
 */
static int Output_Consumer(const void *pOutput, unsigned int nOutputLen, void *pUserData /* Unused */)
{
	/* 
	 * Note that it's preferable to use the write() system call to display the output
	 * rather than using the libc printf() which everybody now is extremely slow.
	 */
	printf("%.*s", 
		nOutputLen, 
		(const char *)pOutput /* Not null terminated */
		);
	/* All done, VM output was redirected to STDOUT */
	return JX9_OK;
}
/*
 * The following walker callback is made available to the [jx9_array_walk()] interface 
 * which is used to iterate over the JSON object extracted from the running script.
 * (See below for more information).
 */
static int JsonObjectWalker(jx9_value *pKey,jx9_value *pData,void *pUserData /* Unused */)
{
	const char *zKey,*zData;
	/* Extract the key and the data field */
	zKey = jx9_value_to_string(pKey,0);
	zData = jx9_value_to_string(pData,0);
	/* Dump */
	printf(
		"%s ===> %s\n",
		zKey,
		zData
	);
	return JX9_OK;
}
/* 
 * Main program: Compile and execute the JX9 program defined above.
 */
int main(int argc,char **argv)
{
	jx9_value *pScalar,*pObject; /* Foreign JX9 variable to be installed later */
	jx9 *pEngine; /* JX9 engine handle*/
	jx9_vm *pVm;  /* Compiled JX9 program */
	int rc;
	int n;

	/* Allocate a new jx9 engine instance */
	rc = jx9_init(&pEngine);
	if( rc != JX9_OK ){
		/*
		 * If the supplied memory subsystem is so sick that we are unable
		 * to allocate a tiny chunk of memory, there is no much we can do here.
		 */
		Fatal("Error while allocating a new JX9 engine instance");
	}
	
	
	/* Compile the test program defined above */
	rc = jx9_compile(pEngine,JX9_PROG,sizeof(JX9_PROG)-1,&pVm);
	if( rc != JX9_OK ){
		if( rc == JX9_COMPILE_ERR ){
			const char *zErrLog;
			int nLen;
			/* Extract error log */
			jx9_config(pEngine, 
				JX9_CONFIG_ERR_LOG, 
				&zErrLog, 
				&nLen
				);
			if( nLen > 0 ){
				/* zErrLog is null terminated */
				puts(zErrLog);
			}
		}
		/* Exit */
		Fatal("Compile error");
	}
	
	/* Register script agruments so we can access them later using the $argv[]
	 * array from the compiled JX9 program.
	 */
	for( n = 1; n < argc ; ++n ){
		jx9_vm_config(pVm, JX9_VM_CONFIG_ARGV_ENTRY, argv[n]/* Argument value */);
	}

	/*
	 * Now we have our script compiled, it's time to configure our VM.
	 * We will install the VM output consumer callback defined above
	 * so that we can consume the VM output and redirect it to STDOUT.
	 */
	rc = jx9_vm_config(pVm, 
		JX9_VM_CONFIG_OUTPUT, 
		Output_Consumer,    /* Output Consumer callback */
		0                   /* Callback private data */
		);
	if( rc != JX9_OK ){
		Fatal("Error while installing the VM output consumer callback");
	}
	
	/* 
	 * Create a simple scalar variable.
	 */
	pScalar = jx9_new_scalar(pVm);
	if( pScalar == 0 ){
		Fatal("Cannot create foreign variable $my_app");
	}
	/* Populate the variable with the desired information */
	jx9_value_string(pScalar,"My Host Application/1.2.5",-1/*Compule length automatically*/); /* Dummy signature*/
	/*
	 * Install the variable ($my_app).
	 */
	rc = jx9_vm_config(
		  pVm,
		  JX9_VM_CONFIG_CREATE_VAR, /* Create variable command */
		  "my_app", /* Variable name (without the dollar sign) */
		  pScalar   /* Value */
		);
	if( rc != JX9_OK ){
		Fatal("Error while installing $my_app");
	}
	/* To access this foreign variable from the running script, simply invoke it
	 * as follows:
	 *  print $my_app;
	 * or
	 *  dump($my_app);
	 */

	/*
	 * Now, it's time to create and install a more complex variable which is a JSON
	 * object named $my_data.
	 * The JSON Object looks like this:
	 *  {
	 *     "path" : "/usr/local/etc",
	 *     "port" : 8082,
	 *     "fork" : true
	 *  };
	 */
	pObject = jx9_new_array(pVm); /* Unified interface for JSON Objects and Arrays */
	/* Populate the object with the fields defined above.
	*/
	jx9_value_reset_string_cursor(pScalar);
	
	/* Add the "path" : "/usr/local/etc" entry */
	jx9_value_string(pScalar,"/usr/local/etc",-1);
	jx9_array_add_strkey_elem(pObject,"path",pScalar); /* Will make it's own copy of pScalar */
	
	/* Add the "port" : 8080 entry */
	jx9_value_int(pScalar,8080);
	jx9_array_add_strkey_elem(pObject,"port",pScalar); /* Will make it's own copy of pScalar */
	
	/* Add the "fork": true entry */
	jx9_value_bool(pScalar,1 /* TRUE */);
	jx9_array_add_strkey_elem(pObject,"fork",pScalar); /* Will make it's own copy of pScalar */

	/* Now, install the variable and associate the JSON object with it */
	rc = jx9_vm_config(
		  pVm,
		  JX9_VM_CONFIG_CREATE_VAR, /* Create variable command */
		  "my_data", /* Variable name (without the dollar sign) */
		  pObject    /*value */
		);
	if( rc != JX9_OK ){
		Fatal("Error while installing $my_data");
	}

	/* Release the two values */
	jx9_release_value(pVm,pScalar);
	jx9_release_value(pVm,pObject);

	/*
	 * And finally, execute our program. Note that your output (STDOUT in our case)
	 * should display the result.
	 */
	jx9_vm_exec(pVm, 0);
	
	/* Extract the content of the variable named $my_config defined in the 
	 * running script which hold a simple JSON object. 
	 */
	pObject = jx9_vm_extract_variable(pVm,"my_config");
	if( pObject && jx9_value_is_json_object(pObject) ){
		/* Iterate over object entries */
		printf("\n\nTotal entries in $my_config = %u\n",jx9_array_count(pObject));
		jx9_array_walk(pObject,JsonObjectWalker,0);
	}
	/* All done, cleanup the mess left behind.
	*/
	jx9_vm_release(pVm);
	jx9_release(pEngine);

	return 0;
}