#include "net.hpp"

char *request(Dict *omega, const char *endpoint)
{
	char *uuid = getUUID();

	Dict alpha[] = {
		{sXOR("hawkid"), uuid},
		{NULL, NULL}};

	Post post = {
		alpha, NULL, omega};

	Data data = {
		SERVER_URL,
		(char *)endpoint,
		SERVER_PORT,
		post};

	// TODO: use actual pinned public key base64
	char *response = fetch(data, fhash(PUBLIC_KEY, strlen(PUBLIC_KEY)));

	return response;
}