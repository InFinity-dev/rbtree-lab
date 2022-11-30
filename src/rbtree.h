#ifndef _RBTREE_H_
#define _RBTREE_H_

#include <stddef.h>

// 노드의 색을 정의
typedef enum { RBTREE_RED, RBTREE_BLACK } color_t;

typedef int key_t;

// 노드 구조체 정의 : node_t
typedef struct node_t {
  color_t color; // 노드의 색
  key_t key; // 노드가 가지는 값
  struct node_t *parent, *left, *right;
  // 노드의 부모노드, Left Child, Right Child 의 주소값을 담는다.
} node_t;

// rbtree -> 루트 노드, nil 노드
typedef struct {
  node_t *root;
  node_t *nil;  // for sentinel
} rbtree;

// 메모리 반환
rbtree *new_rbtree(void);
void delete_rbtree(rbtree *);

// key_t를 키값으로 가진 node_t의 삽입, 검색, 최대값, 최소값 함수
node_t *rbtree_insert(rbtree *, const key_t);
node_t *rbtree_find(const rbtree *, const key_t);
node_t *rbtree_min(const rbtree *);
node_t *rbtree_max(const rbtree *);
int rbtree_erase(rbtree *, node_t *);

// 레드-블랙 트리를 순회하며 배열 형태로 출력
int rbtree_to_array(const rbtree *, key_t *, const size_t);

#endif  // _RBTREE_H_
