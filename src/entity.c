#include <entity.h>

/* Initialize a list with
*/
Entity_t * entity_init(Player_t * entity, int * error) {
    if(entity == NULL || error == NULL) {
        *error = ILLEGAL_ARGUMENT;
        return NULL;
    } else {
        Entity_t * buffer = malloc(sizeof(Entity_t));
        if(buffer == NULL) {
            *error = MALLOC_FAILURE;
            return NULL;
        }
        buffer->next = NULL;
        buffer->entity = entity;
        *error = SUCCESS;
        return buffer;
    }
}
int entity_appendEntity(Entity_t * head, Player_t * ent) {
    if(head == NULL || ent == NULL) {
        return ILLEGAL_ARGUMENT;
    } else {
        Entity_t * tmp = head;
        while(tmp->next != NULL) {
            tmp = tmp->next;
        }
        int * error;
        tmp->next = entity_init(ent, error);
        return SUCCESS;
    }
}
Entity_t * entity_getnthEntity(Entity_t * head, int n, int * error) {
    if(head == NULL) {
        *error = ILLEGAL_ARGUMENT;
        return NULL;
    } else {
        Entity_t * temp = head;
        for(int i = 0; i != n && temp != NULL;i++) {
            temp = temp->next;
        }
        if(temp == NULL) {
            *error = INDEX_NOT_INCLUDED;
            return NULL;
        }
        return temp;
    }
}
// Apply a specific function on all entities in the list:
int entity_applyFunction(Entity_t * head, void (*proc)(Entity_t *, void*, size_t), void* data, size_t dataSize) {
    if(head == NULL || proc == NULL) {
        return ILLEGAL_ARGUMENT;
    } else {
        // TODO
        Entity_t * tmp = head;
        while(tmp->next != NULL) {
            proc(tmp, data, dataSize);
            tmp = tmp->next;
        }
        return SUCCESS;
    }
}