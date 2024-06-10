#include <stdio.h>
#include <stdlib.h>

typedef struct _node {
    int val;
    struct _node* next;
} Node;

typedef struct _nodeWalk_output {
    Node* node;
    _Bool err;
} nodeWalker;

Node* createNode(int val) {
    Node* pNode = (Node*) malloc(sizeof(Node));
    pNode->val = val;
    pNode->next = NULL;
    return pNode;
}

Node* arrToList(int arr[], int size) {
    if (size == 0) {
        return NULL;
    }

    Node* head = createNode(arr[0]);
    Node* node = head;

    for (int i = 1; i < size; i++) {
        node->next = createNode(arr[i]);
        node = node->next;
    }
    return head;
}

void freeList(Node* node) {
    Node* temp;
    while (node != NULL) {
        temp = node;
        node = node->next;
        free(temp);
    }
}

int lenList(Node* node) {
    if (node == NULL) {
        return 0;
    }

    int l = 0;
    while (node != NULL) {
        node = node->next;
        l++;
    }
    return l;
}

nodeWalker _nodeWalk(Node* node, int index, _Bool extend_check) {
    nodeWalker output;
    for (int i = 0; i < index; i++) {
        node = node->next;
        if (extend_check? node->next == NULL : node == NULL) {
            output.node = NULL;
            output.err = 1;
            return output;
        }
    }
    output.node = node;
    output.err = 0;
    return output;
}

int get(Node* head, unsigned int index) {
    if (head == NULL) {
        printf("\nError get(): cannot get value from NULL\n");
        return 0;
    }
    nodeWalker output = _nodeWalk(head, index, 0);
    if (output.err = 1) {
        printf("\nError get(): index %d is out of range", index);
        return 0;
    } 
    return output.node->val;
}

void insert(Node* head, int val, unsigned int index) {
    if (head == NULL) {
        printf("\nError append(): cannot append NULL\n");
        return;
    }

    Node *new;

    if (index == 0) {
        new = createNode(head->val);
        new->next = head->next;
        head->val = val;
        head->next = new;
    } else {
        new = createNode(val);
        nodeWalker output = _nodeWalk(head, index-1, 0);
        if (output.err == 1) {
            printf("Error insert(): index %d is out of range", index);
            return;
        }
        new->next = output.node->next;
        output.node->next = new;
    }
}

void del(Node* head, unsigned int index) {
    if (head == NULL) {
        printf("\nError del(): cannot delete NULL\n");
        return;
    }

    Node* temp; 

    if (index == 0) {
        temp = head->next;
        if (temp == NULL) {
            head->val = 0;
            head->next = NULL;
            return;
        }
        head->val = temp->val;
        head->next = temp->next;
    } else {
        nodeWalker output = _nodeWalk(head, index-1, 1);
        if (output.err == 1) {
            printf("Error del(): index %d is out of range", index);
            return;
        }
        temp = output.node->next;
        output.node->next = temp->next;
    }
    free(temp);
}

void nodeWalk(Node* node) {
    if (node == NULL) {
        printf("\nError nodeWalk(): No value for NULL\n");
        return;
    }
    printf("\n[ ");
    for (int i = 0; node != NULL; i++) {
        printf("%d ", node->val);
        node = node->next;
    }
    printf("]");
}

int main(void) {
    int arr[] = {1, 2, 3, 4, 5};
    Node* head = arrToList(arr, 5);

    nodeWalk(head);

    printf("\n");
    freeList(head);
}
