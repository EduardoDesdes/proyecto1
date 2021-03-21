#ifndef _DEBUG_H
#define _DEBUG_H

#ifdef _DEBUG

#define debug(fmt, args...) \
    do {        \
        fprintf(stdout, "%s %s %d: " fmt "\n", __FILE__, __FUNCTION__, __LINE__, ## args); \
    } while (0)

#else

#define debug(fmt, args...) do { /* empty */ } while (0)

#endif

#define dbgerr(fmt, args...) \
    do {        \
        fprintf(stderr, "%s %s %d: " fmt "\n", __FILE__, __FUNCTION__, __LINE__, ## args); \
    } while (0)
    

#endif
