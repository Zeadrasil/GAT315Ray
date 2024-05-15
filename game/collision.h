#pragma once
#include "raylib.h"

typedef struct ncContact ncContact;
typedef struct ncBody ncBody;

void CreateContacts(ncBody* bodies, ncContact** contacts);
ncContact* GenerateContact(ncBody* body1, ncBody* body2);

void SeparateContacts(ncContact* contacts);
void ResolveContacts(ncContact* contacts);