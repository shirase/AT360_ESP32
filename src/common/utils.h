#if __GNUC__ > 6
#define FALLTHROUGH __attribute__ ((fallthrough))
#else
#define FALLTHROUGH do {} while(0)
#endif

#if !defined(UNUSED)
#define UNUSED(x) (void)(x)
#endif