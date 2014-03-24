/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 * Duda I/O Examples: 001 Hello World 02
 * =====================================
 * Based on the example 001_hello_world, this web service adds a few more
 * callbacks to explain how the Routing maps are processed.
 */

#include "webservice.h"
#include "packages/json/json.h"

DUDA_REGISTER("Duda I/O Examples", "JSON Example");


void cb_info(duda_request_t *dr)
{

    FILE * f = fopen("/proc/uptime","r");
    float uptime, idletime;
    fscanf(f,"%f %f", &uptime, &idletime);
    fclose(f);
    json_t * j_uptime = json->create_object();
    json->add_to_object(j_uptime, "uptime", json->create_number(uptime));
    json->add_to_object(j_uptime, "idletime", json->create_number(idletime));
    
    FILE * g = fopen("/proc/cpuinfo","r");

    char * vendor_id = mem->alloc(100); /* dynamically allocating
                                           memory. Can't use malloc as
                                           stack may use different
                                           memory allocator.
                                        */

    char * model_name = mem->alloc(200);

    gc->add(dr,vendor_id);      /* garbage collector will free
                                   allocated memory after request
                                   context ends */
    gc->add(dr,model_name);
    
    char line[1000];
    while(fgets(line,1000,g) != NULL)
        {
            if(strstr(line,"vendor_id\t:"))
                {
                    sscanf(line, "vendor_id\t: %s", vendor_id);
                }

            if(strstr(line, "model name\t:"))
                {
                    sscanf(line, "model name\t: %[^\n]", model_name);
                    break;
                }
        }

    fclose(g);
    json_t * j_cpu = json->create_object();
    json->add_to_object(j_cpu, "vendor_id", json->create_string(vendor_id));
    json->add_to_object(j_cpu, "model_name", json->create_string(model_name));
    json_t * resp = json->create_object();
    json->add_to_object(resp, "uptime", j_uptime);
    json->add_to_object(resp,"cpuinfo",j_cpu);


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
     * Loading JSON package
     */
    duda_load_package(json, "json");
    
    /*
     * Registering callbacks
     */

    map->static_add("/", "cb_info");

    /* Return, everything is OK */
    return 0;
}
