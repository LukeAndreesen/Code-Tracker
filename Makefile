.PHONY: clean

# Set the paths to the cJSON library installation directory
CJSON_INCLUDE_PATH = /usr/local/include/cjson
CJSON_LIB_PATH = /usr/local/lib

# Add cJSON flags to the compilation command
CJSON_FLAGS = -I$(CJSON_INCLUDE_PATH) -L$(CJSON_LIB_PATH) -lcjson

tracker: tracker.c
	clang -Wall -o tracker tracker.c $(CJSON_FLAGS)

clean:
	rm -f a.out tracker
