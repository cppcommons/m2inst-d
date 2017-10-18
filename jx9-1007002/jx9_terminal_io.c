/*
 * Compile this file together with the jx9 scripting engine source code to generate
 * the executable. For example: 
 *  gcc -W -Wall -O6 -o test jx9_terminal_io.c jx9.c
*/
/*
 * This simple program is a quick introduction on how to embed and start
 * experimenting with the JX9 scripting engine without having to do a lot
 * of tedious reading and configuration.
 *
 * This program ask the user for three random numbers (64-bit integer) using the built-in
 * fgets(), fputs() functions, populate a JSON array with the given numbers, output the raw
 * array, perform a merge-sort using the built-in sort() function and output the result.
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
 * The following is the JX9 program to execute.
 *   
 * //Declare an empty JSON array
 * $num = [];
 *
 * print "Enter a number (Decimal, Hex, Octal or Binary notation)\n";
 * //Populate the JSON array with the first entry
 * $num[] = (int)fgets(STDIN);
 *
 * print "Another number\n";
 * $num[] = (int)fgets(STDIN);

 * print "Finally, one last number\n";
 * $num[] = (int)fgets(STDIN);
 *
 * //show content
 * print "Before sorting\n";
 * print $num;
 *
 * //Sort the array now
 * sort($num);
 * print "\nAfter sorting\n";
 * print $num;
 *
 * A typical output of this program would look like this:
 * Enter a number
 *	50232
 * Another number
 *  80
 * Finally, one last number
 *  0xff
 * Before sorting
 *  [50232,80,255]
 * After sorting
 *  [80,255,50232]
 *
 */
#define JX9_PROG \
    "/*Declare an empty JSON array*/"\
    "$num = [];"\
    "print \"Enter a number (Decimal, Hex, Octal or Binary notation)\n\";"\
    "/*Populate the JSON array with the first entry*/"\
    "$num[] = (int)fgets(STDIN);"\
    "print \"Another number\n\";"\
    "$num[] = (int)fgets(STDIN);"\
    "print \"Finally, one last number\n\";"\
    "$num[] = (int)fgets(STDIN);"\
    "/*show content*/"\
    "print \"Before sorting\n\";"\
    "print $num;"\
    "/*Sort the array now*/"\
    "sort($num);"\
    "print \"\nAfter sorting\n\";"\
    "print $num;"
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
int main(void)
{
	jx9 *pEngine; /* JX9 engine handle*/
	jx9_vm *pVm;  /* Compiled JX9 program */
	int rc;

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