/* inside main, make sure correct number of arguments `n` provided. if
 * not, print `msg` with binary name and quit */
#define ensure_argc(n, msg)	              \
	{ if (argc < n || argc > n) {		  \
			printf(msg, argv[0]);		  \
			return 1;					  \
		}								  \
	}
