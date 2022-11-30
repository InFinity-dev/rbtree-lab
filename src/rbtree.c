#include "rbtree.h"

#include <stdlib.h>

//<Red-Black 트리 Constraint>
//#1. 모든 노드는 RBTREE_RED 혹은 RBTREE_BLACK 속성을 가진다
//#2. 루트 노드는 RBTREE_BLACK
//#3. 모든 nilNode(Leaf)는 RBTREE_BLACK
//#4. RBTREE_RED 노드의 자녀 노드는 RBTREE_BLACK
//     * RBTREE_RED 노드는 연속적으로 존재 할 수 없다
//#5. 임의의 노드에서 자손 nilNode 까지 가는 경로에서 거치게 되는 RBTREE_BLACK 노드의 개수는 같다.
//     *자기 자신은 카운트에서 제외


//rbtree 구조체 생성 함수
rbtree *new_rbtree(void) {
    rbtree *p = (rbtree *) calloc(1, sizeof(rbtree));
    //p에 포인터 통해 접근
    //calloc : void *calloc(n, sizeof(k)) - k의 크기의 변수를 n개 만큼 저장할수 있는 공간 할당후 0으로 초기화
    node_t *nilNode = (node_t *) calloc(1, sizeof(node_t));
    //nilNode calloc 통해 메모리 할당 후 0으로 초기화

    nilNode->color = RBTREE_BLACK;
    //<Red-Black 트리 Constraint #3. 모든 nilNode(Leaf)는 RBTREE_BLACK>
    //    typedef struct {
    //      node_t *root;
    //      node_t *nil;  // for sentinel
    //    } rbtree;


    // 구조체 포인터 : 함수 내부에서 외부에 있는 구조체 변수에 직접 접근하기 위해 구조체 포인터를 쓴다.
    // p의 nil과 root 원소에 구조체 포인터를 통해 접근하여 nilNode로 초기화
    p->nil = nilNode;
    p->root = nilNode;
    return p;
}


//<노드 탐색 시퀀스>
//관련 함수명
//    rbtree_find : key 값을 받아와서 검색
//    rbtree_min : 최소값 노드 반환
//    rbtree_max : 최대값 노드 반환

node_t *rbtree_find(const rbtree *t, const key_t key) {
    node_t *current = t->root;

    while (current != t->nil) {
        if (current->key == key)
            return current;

        if (current->key < key)
            current = current->right;
        else
            current = current->left;
    }

    return NULL;
}

node_t *rbtree_min(const rbtree *t) {
    // TODO: implement find
    if (t->root == t->nil) {
        return NULL;
    }
    node_t *curr = t->root;

    while (curr->left != t->nil) {
        curr = curr->left;
    }
    return curr;
}

node_t *rbtree_max(const rbtree *t) {
    // TODO: implement find
    if (t->root == t->nil) {
        return NULL;
    }

    node_t *curr = t->root;

    while (curr->right != t->nil) {
        curr = curr->right;
    }
    return curr;
}

//<rbtree 회전 함수>
//관련 함수명
//  right_rotation
//  left_rotation
void right_rotation(rbtree *tree, node_t *x) {
    node_t *target;

    target = x->left;
    x->left = target->right;

    if (target->right != tree->nil) {
        target->right->parent = x;
    }

    target->parent = x->parent;

    if (x->parent == tree->nil)
        tree->root = target;
    else if (x == x->parent->left)
        x->parent->left = target;
    else
        x->parent->right = target;

    target->right = x;
    x->parent = target;
}

void left_rotation(rbtree *tree, node_t *x) {
    node_t *target;

    target = x->right;
    x->right = target->left;

    if (target->left != tree->nil) {
        target->left->parent = x;
    }

    target->parent = x->parent;

    if (x->parent == tree->nil)
        tree->root = target;
    else if (x == x->parent->left)
        x->parent->left = target;
    else
        x->parent->right = target;

    target->left = x;
    x->parent = target;
}

//<메모리 Free 시퀀스>
//관련 함수명
//    postorder_freenode
//        노드 삭제시 postorder traversal 통해 (Left Child - Rright Child - Root)
//        재귀 Call 하며 메모리 할당 해제
//    delete_rbtree
void postorder_freenode(rbtree *t, node_t *x) {
    if (x->left != t->nil)
        postorder_freenode(t, x->left);

    if (x->right != t->nil)
        postorder_freenode(t, x->right);

    free(x); // 동적 할당된 메모리 해제
    x = NULL;
}

void delete_rbtree(rbtree *t) {
    if (t->root != t->nil)
        postorder_freenode(t, t->root);
    free(t->nil);
    free(t);
}

//<노드 삽입 & 트리 Fix-Up 시퀀스>
//관련 함수명
//    rbtree_insert_fixup
//        노드 삽입이후 <Red-Black 트리 Constraint>에 의거해 트리 Fix-Up
//        참고 : https://code-lab1.tistory.com/62
//    rbtree_insert

// TODO FIXUP 코멘트 설명
// 삽입 노드 n
void rbtree_insert_fixup(rbtree *t, node_t *n) {

    node_t *uncle;

    while (n->parent->color == RBTREE_RED) {
        // 분기 1 : 삽입노드 n의 부모노드가 조상노드의 왼쪽 서브 트리일 경우
        if (n->parent == n->parent->parent->left) {
            uncle = n->parent->parent->right;

            // 분기 1-1 : 삼촌노드(uncle)가 RED일 경우
            if (uncle->color == RBTREE_RED) {
//                n->parent->color == RBTREE_BLACK;
                uncle->color = RBTREE_BLACK;
                n->parent->parent->color = RBTREE_RED;
                n = n->parent->parent;
            }

            // 분기 1-2 : 삽입노드 n의 삼촌노드(uncle)가 BLACK 이고
            else {
                // 분기 1-2-1 : 삽입노드 n의 삼촌노드(uncle)가 BLACK 이고
                //              - 삽입노드 n이 Right Child일 경우
                if (n == n->parent->right) {
                    n = n->parent;
                    left_rotation(t, n);
                }
                // 분기 1-2-2 : ???
                n->parent->color = RBTREE_BLACK;
                n->parent->parent->color = RBTREE_RED;
                right_rotation(t, n->parent->parent);
            }
        }
        // 분기 2
        else {
            uncle = n->parent->parent->left;

            // 분기 2-1 : 삽입노드 n의 삼촌노드(uncle)가 RED일 경우
            if (uncle->color == RBTREE_RED) {
//                n->parent->color == RBTREE_BLACK;
                uncle->color = RBTREE_BLACK;
                n->parent->parent->color = RBTREE_RED;
                n = n->parent->parent;
            }

            // 분기 2-2 : 삽입노드 n의 삼촌노드(uncle)가 BLACK 이고
            else {
                // 분기 2-2-1 : 삽입노드 n의 삼촌노드(uncle)가 BLACK 이고
                //              - 삽입노드 n이 Left Child일 경우
                if (n == n->parent->left) {
                    n = n->parent;
                    right_rotation(t, n);
                }
                // 분기 2-2-2 : ???
                n->parent->color = RBTREE_BLACK;
                n->parent->parent->color = RBTREE_RED;
                left_rotation(t, n->parent->parent);
            }
        }
    }
    t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
    node_t *y = t->nil;
    node_t *x = t->root;
    node_t *z = (node_t *) calloc(1, sizeof(node_t));

    z->key = key;
    while (x != t->nil) {
        y = x;
        if (z->key < x->key)
            x = x->left;
        else
            x = x->right;
    }

    z->parent = y;

    if (y == t->nil) {
        t->root = z;
    } else if (z->key < y->key) {
        y->left = z;
    } else {
        y->right = z;
    }

    z->left = t->nil;
    z->right = t->nil;
    z->color = RBTREE_RED;

    rbtree_insert_fixup(t, z);

    //return z;
    return t->root;
}

// <노드 삭제 & 트리 Fix-Up 시퀀스>
//관련 함수명
//    rbtree_erase
//    rbtree_erase_fixup
//        노드 삭제 이후 <Red-Black 트리 Constraint>에 의거해 트리 Fix-Up
//    rbtree_transplant
//        삭제된 노드 이하 서브 트리 붙여주는 함수
// TODO FIXUP 코멘트 설명
void rbtree_erase_fixup(rbtree *t, node_t *x) {
    while (x != t->root && x->color == RBTREE_BLACK) {
        // 분기 1 : Left
        if (x == x->parent->left) {
            node_t *w = x->parent->right;

            // 분기 1-1 : x의 형제 w가 적색인 경우
            if (w->color == RBTREE_RED) {
                w->color = RBTREE_BLACK;
                x->parent->color = RBTREE_RED;
                left_rotation(t, x->parent);
                w = x->parent->right;
            }
            // 분기 1-2 : x의 형제 w는 흑색이고 w의 두 지식이 모두 흑색인 경우
            if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
                w->color = RBTREE_RED;
                x = x->parent;
            }
                // 분기 1-3 : w = BLACK
            else {
                //분기 1-3-1  : w의 왼쪽 자식은 적색, w의 오른쪽 자신은 흑색인 경우
                if (w->right->color == RBTREE_BLACK) {
                    w->left->color = RBTREE_BLACK;
                    w->color = RBTREE_RED;
                    right_rotation(t, w);
                    w = x->parent->right;
                }

                // 분기 1-3-2 : w의 오른쪽 자식은 적색인 경우
                w->color = x->parent->color;
                x->parent->color = RBTREE_BLACK;
                w->right->color = RBTREE_BLACK;
                left_rotation(t, x->parent);
                x = t->root;
            }
        }
        // 분기 2 : Right
        else {
            node_t *w = x->parent->left;

            // 분기 2-1 : x의 형제 w가 적색인 경우
            if (w->color == RBTREE_RED) {
                w->color = RBTREE_BLACK;
                x->parent->color = RBTREE_RED;
                right_rotation(t, x->parent);
                w = x->parent->left;
            }
            // 분기 2-2 : x의 형제 w는 흑색이고 w의 두 지식이 모두 흑색인 경우
            if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
                w->color = RBTREE_RED;
                x = x->parent;
            }
            // 분기 2-3 : w = BLACK
            else {
                // 분기 2-3-1 : w의 왼쪽 자식은 적색, w의 오른쪽 자신은 흑색인 경우
                if (w->left->color == RBTREE_BLACK) {
                    w->right->color = RBTREE_BLACK;
                    w->color = RBTREE_RED;
                    left_rotation(t, w);
                    w = x->parent->left;
                }

                // 분기 2-3-2 : w의 오른쪽 자식은 적색인 경우
                w->color = x->parent->color;
                x->parent->color = RBTREE_BLACK;
                w->left->color = RBTREE_BLACK;
                right_rotation(t, x->parent);
                x = t->root;
            }
        }
    }
    x->color = RBTREE_BLACK;
}

void rbtree_transplant(rbtree *t, node_t *u, node_t *v) {
    if (u->parent == t->nil) {
        t->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }

    v->parent = u->parent;
}

int rbtree_erase(rbtree *t, node_t *p) {
    node_t *y;
    node_t *x;
    color_t yOriginalColor;

    y = p;
    yOriginalColor = y->color;

    if (p->left == t->nil) {
        x = p->right;
        rbtree_transplant(t, p, p->right);
    } else if (p->right == t->nil) {
        x = p->left;
        rbtree_transplant(t, p, p->left);
    } else {
        y = p->right;
        while (y->left != t->nil) {
            y = y->left;
        }
        yOriginalColor = y->color;
        x = y->right;

        if (y->parent == p) {
            x->parent = y;
        } else {
            rbtree_transplant(t, y, y->right);
            y->right = p->right;
            y->right->parent = y;
        }

        rbtree_transplant(t, p, y);
        y->left = p->left;
        y->left->parent = y;
        y->color = p->color;
    }

    if (yOriginalColor == RBTREE_BLACK) {
        rbtree_erase_fixup(t, x);
    }

    free(p);

    return 0;
}

//<트리 출력 시퀀스(배열 형태로 평탄화)>
//관련 함수명
//subtree_to_array
//rbtree_to_array
void subtree_to_array(const rbtree *t, node_t *curr, key_t *arr, size_t n, size_t *count) {
    if (curr == t->nil) {
        return;
    }

    subtree_to_array(t, curr->left, arr, n, count);

    if (*count < n) {
        arr[(*count)++] = curr->key;
    } else return;

    subtree_to_array(t, curr->right, arr, n, count);
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
    if (t->root == t->nil) {
        return 0;
    }
    size_t cnt = 0;
    subtree_to_array(t, t->root, arr, n, &cnt);

    return 0;
}