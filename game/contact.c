#include "contact.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>


void AddContact(ncContact* contact, ncContact** contacts)
{
	assert(contact);

	// add element to linked list
	contact->next = *contacts;

	// set head of elements to new element
	*contacts = contact;
}

void DestroyAllContacts(ncContact** contacts)
{
	if (!*contacts) return;

	ncContact* contact = *contacts;
	while (contact)
	{
		ncContact* next = contact->next;
		free(contact);
		contact = next;
	}

	*contacts = NULL;
}