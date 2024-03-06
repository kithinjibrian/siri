#include "linked.hpp"

void unshift(Node **head_ref, Meta data)
{
	Node *new_node = (Node *)malloc(sizeof(Node));
	new_node->data = data;
	new_node->next = *head_ref;
	*head_ref = new_node;
}

void printNode(Node *node)
{
	while (node != NULL)
	{
		printf("%s -> ", node->data.filename);
		node = node->next;
	}
	printf("NULL\n");
}