/*
 * Compile this file together with the jx9 scripting engine source code to generate
 * the executable. For example: 
 *  gcc -W -Wall -O6 -o test jx9_mp3_tag.c jx9.c
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
 * The following is the JX9 program to execute. The program expects a command line
 * path from which all available MP3 are extrcated and their ID3v1 tags (if available)
 * are outputted.
 * To run this program, simply enter the executable name and the target directory as follows:
 *
 *      test_prog /path/to/my/mp3s
 * 
 *  if( count($argv) < 1 ){
 *   EXIT("Missing MP3s directory");
 * }
 * $dir = $argv[0];
 * if( !chdir($dir) ){
 *	 EXIT("Error while changing directory");
 * }
 * //Collect MP3 files
 * $pFiles = glob("*.mp3");
 * if( is_array($pFiles) ){
 *  foreach($pFiles as $pEntry){
 *    print "\nMP3: $pEntry ",size_format(filesize($pEntry)),JX9_EOL;// Give a nice presentation
 *	 // Open the file in a read only mode
 *	 $pHandle = fopen($pEntry,'r');
 *	 if( $pHandle == FALSE ){
 *	   print "IO error while opening $pEntry\n";
 *	   continue; // Ignore 
 *	 }
 *	 // Seek 128 bytes from the end
 *	 fseek($pHandle,-128,SEEK_END);
 *	 // Read the 128 raw data
 *	 $zBuf = fread($pHandle,128);
 *	 if( !$zBuf || strlen($zBuf) != 128 ){
 *		print "$pEntry: Read error\n";
 *		// Ignore
 *		continue;
 *	 }
 *	 if( $zBuf[0] == 'T' && $zBuf[1] == 'A' && $zBuf[2] == 'G'  ){
 *	    $nOfft = 3;
 *		// Extract the title
 *		$zTitle = substr($zBuf,$nOfft,30);
 *		// Remove trailing and leading NUL bytes and white spaces
 *		$zTitle = trim($zTitle);
 *		if( strlen($zTitle) > 0 ){
 *				print "Title: $zTitle\n";
 *		}
 *		// Extract artist name
 *		$zArtist = substr($zBuf,$nOfft+30,30);
 *		// Remove trailing and leading NUL bytes and white spaces
 *		$zArtist = trim($zArtist);
 *		if( strlen($zArtist) > 0 ){
 *				print "Artist: $zArtist\n";
 *		}
 *		// Extract album name 
 *		$zAlbum = substr($zBuf,$nOfft+30+30,30);
 *		// Remove trailing and leading NUL bytes and white spaces
 *		$zAlbum = trim($zAlbum);
 *		if( strlen($zAlbum) > 0 ){
 *				print "Album: $zAlbum\n";
 *		}
 *		// Extract the Year 
 *		$zYear = substr($zBuf,$nOfft+30+30+30,4);
 *		// Remove trailing and leading NUL bytes and white spaces
 *		$zYear = trim($zYear);
 *		if( strlen($zYear) > 0 ){
 *				print "Year: $zYear\n";
 *		}
 *		// Next entry
 *		print "------------------------------------------------------\n";
 *	 }
 *	 // All done whith this file, close the handle
 *	 fclose($pHandle);
 *  }
 * }
 *
 */
#define JX9_PROG \
  "if( count($argv) < 1 ){"\
  " EXIT('Missing MP3s directory');"\
  "}"\
  "$dir = $argv[0];"\
  "if( !chdir($dir) ){"\
  "	EXIT('Error while changing directory');"\
  "}"\
  "/* Collect MP3 files */"\
  "$pFiles = glob('*.mp3');"\
  "if( is_array($pFiles) ){"\
  "foreach($pFiles as $pEntry){"\
  "   print \"\nMP3: $pEntry \",size_format(filesize($pEntry)),JX9_EOL;/* Give a nice presentation */"\
  "	 /* Open the file in a read only mode */"\
  "	 $pHandle = fopen($pEntry,'r');"\
  "	 if( $pHandle == FALSE ){"\
  "	   print \"IO error while opening $pEntry\n\";"\
  "	   continue; /* Ignore */"\
  "	 }"\
  "	 /* Seek 128 bytes from the end */"\
  "	 fseek($pHandle,-128,SEEK_END);"\
  "	 /* Read the 128 raw data */"\
  "	 $zBuf = fread($pHandle,128);"\
  "	 if( !$zBuf || strlen($zBuf) != 128 ){"\
  "		print \"$pEntry: Read error\n\";"\
  "		/* Ignore */"\
  "		continue;"\
  "	 }"\
  "	 if( $zBuf[0] == 'T' && $zBuf[1] == 'A' && $zBuf[2] == 'G'  ){"\
  "	    $nOfft = 3 /* TAG */;"\
  "		/* Extract the title */"\
  "		$zTitle = substr($zBuf,$nOfft,30);"\
  "		/* Remove trailing and leading NUL bytes and white spaces */"\
  "		$zTitle = trim($zTitle);"\
  "		if( strlen($zTitle) > 0 ){"\
  "				print \"Title: $zTitle\n\";"\
  "		}"\
  "		/* Extract artist name */"\
  "		$zArtist = substr($zBuf,$nOfft+30,30);"\
  "		/* Remove trailing and leading NUL bytes and white spaces */"\
  "		$zArtist = trim($zArtist);"\
  "		if( strlen($zArtist) > 0 ){"\
  "				print \"Artist: $zArtist\n\";"\
  "		}"\
  "		/* Extract album name */"\
  "		$zAlbum = substr($zBuf,$nOfft+30+30,30);"\
  "		/* Remove trailing and leading NUL bytes and white spaces */"\
  "		$zAlbum = trim($zAlbum);"\
  "		if( strlen($zAlbum) > 0 ){"\
  "				print \"Album: $zAlbum\n\";"\
  "		}"\
  "		/* Extract the Year */"\
  "		$zYear = substr($zBuf,$nOfft+30+30+30,4);"\
  "		/* Remove trailing and leading NUL bytes and white spaces */"\
  "		$zYear = trim($zYear);"\
  "		if( strlen($zYear) > 0 ){"\
  "				print \"Year: $zYear\n\";"\
  "		}"\
  "		/* Next entry */"\
  "		print \"------------------------------------------------------\n\";"\
  "	 }"\
  "	 /* All done whith this file,close the handle */"\
  "	 fclose($pHandle);"\
  " }"\
  "}"
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