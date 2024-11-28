#define HOSTED

#ifdef HOSTED
#define NW_HTTP std::string("http://localhost:8080")
#define NW_WS std::string("ws://localhost:8081")
#else
#define NW_HTTP std::string("http://localhost:8080")
#define NW_WS std::string("ws://localhost:8081")
#endif
