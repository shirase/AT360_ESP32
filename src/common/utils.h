#if __GNUC__ > 6
#define FALLTHROUGH __attribute__ ((fallthrough))
#else
#define FALLTHROUGH do {} while(0)
#endif