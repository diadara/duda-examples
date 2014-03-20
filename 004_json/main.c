/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 * Duda I/O Examples: 001 Hello World 02
 * =====================================
 * Based on the example 001_hello_world, this web service adds a few more
 * callbacks to explain how the Routing maps are processed.
 */

#include "webservice.h"
#include "packages/json/json.h"

DUDA_REGISTER("Duda I/O Examples", "Hello World");


void cb_json(duda_request_t *dr)
{

    FILE * f = fopen("/proc/uptime","r");
    
    float uptime, idletime;
    fscanf(f,"%f %f", &uptime, &idletime);
    fclose(f);
    json_t * resp = json->create_object();
    json->add_to_object(resp, "uptime", json->create_number(uptime));
    json->add_to_object(resp, "idletime", json->create_number(idletime));
    unsigned long * len = malloc(sizeof *len);
    response->http_status(dr,200);
    response->http_content_type(dr, "json");
    response->printf(dr, json->print_gc(dr,resp));
    response->end(dr, NULL);

        
}

/*
 * This is the principal function loaded by Duda stack to initialize
 * the web service, it's called just one time and if it returns
 * zero it will be registered, otherwise skipped.
 */
int duda_main()
{

    /*
     * Registering callbacks
     * ---------------------
     * Here we will define three callbacks to match three different
     * type of requests, the goal is to understand how the matching
     * rules works.
     */
        duda_load_package(json, "json");
   
   
    map->static_add("/", "cb_json");
    /* Return, everything is OK */
    return 0;
}
