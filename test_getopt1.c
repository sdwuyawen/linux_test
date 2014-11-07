/**
 * 	@filename :
 * 		test_getopt1.c
 * 
 * 	@note : 参考test_getopt.c中的函数说明
 *
 **/
#include <stdio.h>
#include <string.h>
#include <getopt.h>

#define MAX_INPUT_PLUGINS 10
#define MAX_OUTPUT_PLUGINS 10

static struct option long_options[] = {
	{"h", no_argument, 0, 0},
	{"help", no_argument, 0, 0},
	{"i", required_argument, 0, 0},
	{"input", required_argument, 0, 0},
	{"o", required_argument, 0, 0},
	{"output", required_argument, 0, 0},
	{"v", no_argument, 0, 0},
	{"version", no_argument, 0, 0},
	{"b", no_argument, 0, 0},
	{"background", no_argument, 0, 0},
	{0, 0, 0, 0}
};

void help(char *progname)
{
    fprintf(stderr, "-----------------------------------------------------------------------\n");
    fprintf(stderr, "Usage: %s\n" \
            "  -i | --input \"<input-plugin.so> [parameters]\"\n" \
            "  -o | --output \"<output-plugin.so> [parameters]\"\n" \
            " [-h | --help ]........: display this help\n" \
            " [-v | --version ].....: display version information\n" \
            " [-b | --background]...: fork to the background, daemon mode\n", progname);
    fprintf(stderr, "-----------------------------------------------------------------------\n");
    fprintf(stderr, "Example #1:\n" \
            " To open an UVC webcam \"/dev/video1\" and stream it via HTTP:\n" \
            "  %s -i \"input_uvc.so -d /dev/video1\" -o \"output_http.so\"\n", progname);
    fprintf(stderr, "-----------------------------------------------------------------------\n");
    fprintf(stderr, "Example #2:\n" \
            " To open an UVC webcam and stream via HTTP port 8090:\n" \
            "  %s -i \"input_uvc.so\" -o \"output_http.so -p 8090\"\n", progname);
    fprintf(stderr, "-----------------------------------------------------------------------\n");
    fprintf(stderr, "Example #3:\n" \
            " To get help for a certain input plugin:\n" \
            "  %s -i \"input_uvc.so --help\"\n", progname);
    fprintf(stderr, "-----------------------------------------------------------------------\n");
    fprintf(stderr, "In case the modules (=plugins) can not be found:\n" \
            " * Set the default search path for the modules with:\n" \
            "   export LD_LIBRARY_PATH=/path/to/plugins,\n" \
            " * or put the plugins into the \"/lib/\" or \"/usr/lib\" folder,\n" \
            " * or instead of just providing the plugin file name, use a complete\n" \
            "   path and filename:\n" \
            "   %s -i \"/path/to/modules/input_uvc.so\"\n", progname);
    fprintf(stderr, "-----------------------------------------------------------------------\n");	
}

int main(int argc, char *argv[])
{
    char *input[MAX_INPUT_PLUGINS];
    char *output[MAX_OUTPUT_PLUGINS];
    int incnt = 0;
    int outcnt = 0;
    int daemon = 0, i;
    int option_index = 0, c = 0;
	
    /* parameter parsing */
    while(1) {
        c = getopt_long_only(argc, argv, "", long_options, &option_index);
		printf("c=%d, option_index=%d\n", c, option_index);
		
        /* no more options to parse */
        if(c == -1) break;
        
        /* unrecognized option */
        if(c == '?') {
            help(argv[0]);
            return 0;
        }

        switch(option_index) {
            /* h, help */
        case 0:
        case 1:
            help(argv[0]);
            return 0;
            break;

            /* i, input */
        case 2:
        case 3:
            input[incnt++] = strdup(optarg);
            break;

            /* o, output */
        case 4:
        case 5:
            output[outcnt++] = strdup(optarg);
            break;

            /* v, version */
        case 6:
        case 7:
            printf("" \
            "Compilation Date.....: %s\n" \
            "Compilation Time.....: %s\n",
            __DATE__, __TIME__);
            return 0;
            break;

            /* b, background */
        case 8:
        case 9:
            daemon = 1;
            printf("daemon\n");
            break;

        default:
            help(argv[0]);
            return 0;
        }
    }	
        
    if (incnt) {
		for (i=0; i<incnt; i++) {
			printf("in[%d]: %s\n", i, input[i]);
		}	
	}
	
    if (outcnt) {
		for (i=0; i<outcnt; i++) {
			printf("out[%d]: %s\n", i, output[i]);
		}			
	}	
    
	return 0;
}
