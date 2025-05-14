#include "client.h"
#include "lua.h"
#include "split.h"
#include "window.h"
#include "debug.h"

#include <stdlib.h>

typedef struct
{
	RoseClient **clients;
	int n_clients;
} RoseClients;

RoseClients *rose_client_get_all(void)
{
	static RoseClients clients = { NULL, 0 };
	return &clients;
}

RoseClient *rose_client_new(void)
{
	RoseClients *rcs = rose_client_get_all();
	RoseClient *rc = calloc(sizeof(*rc), 1);

	rc->window = rose_window_get();
	rc->id = rand();

	while (rose_client_get_by_id(rc->id)) {
		rc->id = rand();
	}

	rcs->clients = realloc(rcs->clients, sizeof(RoseClient*) * (rcs->n_clients + 1));
	rcs->clients[rcs->n_clients++] = rc;

	return rc;
}

RoseClient *rose_client_get_by_window(RoseWindow *window)
{
	RoseClients *rcs = rose_client_get_all();

	for (int i = 0; i < rcs->n_clients; i++) {
		if (rcs->clients[i]->window == window)
			return rcs->clients[i];
	}

	return NULL;
}

RoseClient *rose_client_get_by_id(int id)
{
	RoseClients *rcs = rose_client_get_all();

	for (int i = 0; i < rcs->n_clients; i++) {
		if (rcs->clients[i]->id == id)
			return rcs->clients[i];
	}

	return NULL;
}

void rose_client_destroy_by_window(RoseWindow *window)
{
	RoseClients *rcs = rose_client_get_all();

	for (int i = 0; i < rcs->n_clients; i++) {
		if (rcs->clients[i]->window == window) {
			debug("Destroying window (id: %i)", rcs->clients[i]->id);
			free(rcs->clients[i]);
			rcs->clients[i] = NULL;

			for (int j = i + 1; j < rcs->n_clients; j++) {
				rcs->clients[j - 1] = rcs->clients[j];
			}

			rcs->n_clients--;
			rcs->clients = realloc(rcs->clients, rcs->n_clients * sizeof(RoseClient*));

			if (rcs->n_clients == 0)
				exit(0);
		}
	}
}

void rose_client_destroy_by_id(int id)
{
	RoseClients *rcs = rose_client_get_all();

	for (int i = 0; i < rcs->n_clients; i++) {
        if (rcs->clients[i]->id == id) {
			debug("Destroying window (id: %i)", rcs->clients[i]->id);
            free(rcs->clients[i]);
            rcs->clients[i] = NULL;

            for (int j = i + 1; j < rcs->n_clients; j++) {
                rcs->clients[j - 1] = rcs->clients[j];
            }

            rcs->n_clients--;
            rcs->clients = realloc(rcs->clients, rcs->n_clients * sizeof(RoseClient*));

			if (rcs->n_clients == 0)
				exit(0);
            return;
        }
    }
}

int rose_client_lua_new(lua_State *L)
{
	RoseClient *rc = rose_client_new();
	lua_pushlightuserdata(L, rc);
	return 1;
}

void rose_client_lua_api(lua_State *L)
{
	rose_lua_table_add_field("rose.client");
	lua_pushcfunction(L, rose_client_lua_new);
	lua_setfield(L, -2, "new");
	lua_pop(L, -2);
}

void rose_client_destroy_all(void)
{
	RoseClients *rcs = rose_client_get_all();

	for (int i = 0; i < rcs->n_clients; i++) {
		rose_window_destroy(rcs->clients[i]->window);
	}

	free(rcs->clients);
}
