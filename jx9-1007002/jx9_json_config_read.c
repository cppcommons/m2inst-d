/*
 * Compile this file together with the jx9 scripting engine source code to generate
 * the executable. For example: 
 *  gcc -W -Wall -O6 -o test jx9_json_config_read.c jx9.c
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
 * The following JX9 program demonstrate how the JX9 scripting engine can be used
 * as a configuration file processor for a host application. This program
 * open the JSON file (the configuration) created by jx9_json_config_write.c 
 * and parse its contents into a name value pair using the built-in json_decode().
 * 
 * JX9 Program:
 *
 * $zFile = 'config.json.txt'; // Name of the configuration file 
 * if( count($argv) > 0 && is_string($argv[0]) ){
 *  // Take the filename from the command line
 *  $zFile = $argv[0];
 * }
 * // Open the configuration file created with jx9_json_config_write.c
 * $iSz = filesize($zFile); // Size of the whole file using the builtin function filesize()
 * $fp = fopen($zFile,'r'); // Read-only mode
 * if( !$fp ){
 *	exit("Cannot open '$zFile'");
 * }
 * // Read the raw content
 * $zBuf = fread($fp,$iSz);
 * // decode the JSON object now
 * $my_config = json_decode($zBuf);
 * if( !$my_config ){
 *	print "JSON decoding error\n";
 * }else{
 * // Dump the JSON object
 * foreach( $my_config as $key,$value ){
 *   print "$key ===> $value\n";
 *  }
 * }
 * // Close the file
 * fclose($fp);
 *   
 */
#define JX9_PROG \
  "$zFile = 'config.json.txt'; /* Name of the configuration file */"\
  "if( count($argv) > 0 && is_string($argv[0]) ){"\
  " /* Take the filename from the command line*/"\
  " $zFile = $argv[0];"\
  "}"\
  "/* Open the configuration file created with jx9_json_config_write.c */"\
  "$iSz = filesize($zFile); /* Size of the whole file using the builtin function filesize() */"\
  "$fp = fopen($zFile,'r'); /* Read-only mode */"\
  "if( !$fp ){"\
  "	exit(\"Cannot open '$zFile'\");"\
  "}"\
  "/* Read the raw content */"\
  "$zBuf = fread($fp,$iSz);"\
  "/* decode the JSON object now */"\
  "$my_config = json_decode($zBuf);"\
  "if( !$my_config ){"\
  "	print \"JSON decoding error\n\";"\
  "}else{"\
  "/* Dump the JSON object */"\
  "foreach( $my_config as $key,$value ){"\
  "  print \"$key ===> $value\n\";"\
  "}"\
  "}"\
  "/* Close the file */"\
  "fclose($fp);"
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
 * Main program: Compile and execute the JX9 program defined above.
 */
int main(int argc,char **argv)
{
	jx9 *pEngine; /* JX9 engine handle*/
	jx9_vm *pVm;  /* Compiled JX9 program */
	int i,rc;

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

	/* Register script arguments (the file to be processed) */
	for( i = 1; i < argc ; ++i ){
		jx9_vm_config(pVm,JX9_VM_CONFIG_ARGV_ENTRY,argv[i]);
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
	 * And finally, execute our program. Note that your output (STDOUT in our case)
	 * should display the result.
	 */
	jx9_vm_exec(pVm, 0);
	
	
	/* All done, cleanup the mess left behind.
	*/
	jx9_vm_release(pVm);
	jx9_release(pEngine);

	return 0;
}