#include "rbtree.h"
#include <stdlib.h>
#include <stdio.h>

void rbtree_fixup(rbtree *t, node_t *current);
void delete_fixup(rbtree *t, node_t* target_sub);
void delete_rbtree(rbtree *t);
void delete_midorder(rbtree *t, node_t* node);

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *nil = (node_t *)calloc(1, sizeof(node_t));
  nil->color = RBTREE_BLACK;
  nil->left = NULL;
  nil->right = NULL;
  nil->parent = NULL;

  p->root = nil;
  p->nil = nil;

  return p;
}

void delete_midorder(rbtree *t, node_t* node){
  // node_t* node = t->root;
  if(node == t->nil){
    return;
  }
  delete_midorder(t, node->left);
  delete_midorder(t, node->right);
  free(node);
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  if(t->root == t->nil){
    free(t->nil);
    return;
  }
  delete_midorder(t, t->root);
  free(t->nil);
  free(t);
  return;
}

//회전하는 함수
void left_rotate(rbtree *t, node_t *current){
  node_t *y = current->right;
  current->right = y->left;
  if(y->left != t->nil)
    y->left->parent = current;

  y->parent = current->parent;

  if(current->parent == t->nil) //노드 부모가 가리키는게 닐노드이면
    t->root = y; // y를 루트로
  else if(current == current->parent->left) 
    current->parent->left = y;
  else{
    current->parent->right = y;
  }
  y->left = current;
  current->parent = y;
  return;
}

void right_rotate(rbtree *t, node_t *current){
  node_t *y =  current->left;
  current->left = y->right;
  
  if(y->right != t->nil)
    y->right->parent = current;
  
  y->parent = current->parent;

  if(current->parent == t->nil) //노드 부모가 가리키는게 닐노드이면
    t->root = y; // y를 루트로
  else if(current == current->parent->right) 
    current->parent->right = y;
  else{
    current->parent->left = y;
  }
  y->right = current;
  current->parent = y;
  return;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  
  //루트노드가 현재노드
  node_t *current = t->root;
  node_t *parent = t->nil;

  while(current != t->nil){ //루트노드가 nil이 아니고 값이 있으면
    parent = current;
    if(current->key > key) //들어온 키가 작으면
      current = current->left;
    else
      current = current->right;
  }

  //새 노드 생성
  node_t *new_node = (node_t *)calloc(1, sizeof(node_t));
  
  new_node->key = key;
  new_node->color = RBTREE_RED;
  new_node->parent = parent;
  new_node->left = t->nil;
  new_node->right = t->nil;

  if(parent == t->nil){ //root노드가 nil이면
      t->root = new_node;
      new_node->color = RBTREE_BLACK;
  }
  else if(parent->key < new_node->key)
    parent->right = new_node;
  else
    parent->left = new_node;
  

  // fixup
  rbtree_fixup(t,new_node);

  return t->root;
}

void rbtree_fixup(rbtree *t, node_t *z){
 node_t *uncle;
    while (z->parent->color == RBTREE_RED){
        if (z->parent == z->parent->parent->left){
            uncle = z->parent->parent->right;
            //경우1
            if (uncle->color == RBTREE_RED){
                z->parent->color = RBTREE_BLACK;
                uncle->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                z = z->parent->parent;
            }
            //경우2 (경우 2는 경우 3을 경유)
            else {
                if (z == z->parent->right){
                    z = z->parent;
                    left_rotate(t, z);
                }
                //경우3
                z->parent->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                right_rotate(t, z->parent->parent);
            }
        }
        //반대로
        else
        {
            uncle = z->parent->parent->left;
            //경우1
            if (uncle->color == RBTREE_RED){
                z->parent->color = RBTREE_BLACK;
                uncle->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                z = z->parent->parent;
            }
            //경우2 (경우2는 경우3을 경유)
            else {
                if (z == z->parent->left){
                    z = z->parent;
                    right_rotate(t, z);
                }
                //경우3
                z->parent->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                left_rotate(t, z->parent->parent);
            }
        }
    }
    t->root->color = RBTREE_BLACK;
}


node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t* current = t->root;
  
  while(current != t->nil){
    if(current->key == key){
      return current;
    }
    else if(current->key < key){ //key가 더 크면
      current = current->right;
    }
    else{ //key가 작으면
      current = current->left; 
    }
  }
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t* node = t->root;
  if(node == t->nil)
    return node;
  while(node->left != t->nil){
    node = node->left;
  }
  return node;
}

node_t* tree_minimum(const rbtree *t, node_t* node){
  if(node == t->nil)
    return node;
  while(node->left != t->nil){
    node = node->left;
  }
  return node;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t* node = t->root;
  if(node == t->nil)
    return node;
  while(node->right !=  t->nil){
    node = node->right;
  }
  return node;
}
//대체할 노드 설정
void transplant(rbtree *t, node_t *target, node_t *target_sub){
  //삭제할 노드가 root면
  if(target->parent == t->nil){
    t->root = target_sub;
  }
  else if(target == target->parent->left){ //부모 왼쪽이 삭제할 노드면
    target->parent->left = target_sub; //왼쪽에 서브를 대체함.
  }
  else{ //부모 오른쪽자식이 삭제할 노드면
    target->parent->right = target_sub; //오른쪽에 서브를 대체.
  }
  //대체하는 노드 부모를 삭제노드 부모로
  target_sub->parent = target->parent;
  return;
}

//원하는 노드 지우기
int rbtree_erase(rbtree *t, node_t *target) {
  //TODO: implement erase
  node_t* successor = target; //subnode에 복사, 삭제할 노드의 위치를 알아야하니까.
  color_t target_origin_color = successor->color; //삭제할 노드의 색 저장
  node_t* target_sub; //대체할 노드를 x에 저장

  if(target->left == t->nil){ //왼쪽 자식이 없을 떄
    target_sub = target->right;
    transplant(t, target, target->right); //타겟 오른쪽 자식이 타겟노드를 대체함.
  }
  //target노드 오른쪽 자식이 없을 때
  else if(target->right == t->nil){ 
    target_sub = target->left;
    transplant(t, target, target->left); //타겟 왼쪽 자식이 타겟을 대체해줌.
  }
  //삭제하려는 노드 자식이 둘 다 있는 경우
  else{
    successor = tree_minimum(t, target->right); //서브타겟 successor
    target_origin_color = successor->color; //successor색 저장

    target_sub = successor->right; //서브타겟을 successor노드 오른쪽자식으로(successor를 삭제함)

    if(successor->parent == target){ //succesor의 부모가 타겟이면
      target_sub->parent = successor; //successor 오른쪽자식의 부모노드를 successor로 설정.
    }
    else{
      transplant(t, successor, successor->right); //successor 오른쪽 자식이 successor를 대체함.
      successor->right = target->right; 
      successor->right->parent = successor;
    }
    transplant(t, target, successor);
    successor->left = target->left;
    successor->left->parent = successor;
    successor->color = target->color;
  }

  if(target_origin_color == RBTREE_BLACK){
    delete_fixup(t, target_sub);
  }
  free(target);
  
  return 0;
}

void delete_fixup(rbtree *t, node_t* target_sub){
  //대체될 노드의 색이 black or 루트가 아니면
  while(target_sub != t->root && target_sub->color == RBTREE_BLACK){ 
    if(target_sub == target_sub->parent->left){
      node_t *brother = target_sub->parent->right; //형제
      if(brother->color == RBTREE_RED){ //형제가 red면 형제를 black만든 후 case 2, 3, 4중 해결
        brother->color = RBTREE_BLACK;
        target_sub->parent->color = RBTREE_RED; //부모는 red로
        left_rotate(t,target_sub->parent); //부모기준 왼쪽으로 회전
        brother = target_sub->parent->right;
      }
      //case 2 형제왼쪽색이랑, 오른쪽 색이 검은색일 때
      if(brother->left->color == RBTREE_BLACK && brother->right->color == RBTREE_BLACK){
        brother->color = RBTREE_RED;
        target_sub = target_sub->parent;
      }
      else{
        if(brother->right->color == RBTREE_BLACK){
            brother->left->color = RBTREE_BLACK;
            brother->color = RBTREE_RED;
            right_rotate(t, brother);
            brother = target_sub->parent->right;
          }
          brother->color = target_sub->parent->color;
          target_sub->parent->color = RBTREE_BLACK;
          brother->right->color = RBTREE_BLACK;
          left_rotate(t, target_sub->parent);
          target_sub = t->root;
        }
      }
      else{
        node_t *brother = target_sub->parent->left; //형제
        if(brother->color == RBTREE_RED){ //형제가 red면 형제를 black만든 후 case 2, 3, 4중 해결
          brother->color = RBTREE_BLACK;
          target_sub->parent->color = RBTREE_RED; //부모는 red로
          right_rotate(t,target_sub->parent); //부모기준 왼쪽으로 회전
          brother = target_sub->parent->left;
        }
        //case 2 형제왼쪽색이랑, 오른쪽 색이 검은색일 때
        if(brother->right->color == RBTREE_BLACK && brother->left->color == RBTREE_BLACK){
          brother->color = RBTREE_RED;
          target_sub = target_sub->parent;
        }
        else{ 
          if(brother->left->color == RBTREE_BLACK){
            brother->right->color = RBTREE_BLACK;
            brother->color = RBTREE_RED;
            left_rotate(t, brother);
            brother = target_sub->parent->left;
          }
          brother->color = target_sub->parent->color;
          target_sub->parent->color = RBTREE_BLACK;
          brother->left->color = RBTREE_BLACK;
          right_rotate(t, target_sub->parent);
          target_sub = t->root;
        }
      }
    }
  target_sub->color = RBTREE_BLACK;
}

void inoroder_arr(const rbtree *t, node_t *node, key_t *arr, int *i){
  if(node == t->nil){
    return;    
  }
  inoroder_arr(t, node->left, arr, i);
  arr[*i] = node->key;
  *i = *i + 1;
  inoroder_arr(t, node->right, arr, i);
  
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  if(t->root == t->nil){
    return 0;
  }
  int idx=0;
  int *i;
  i= &idx;

  inoroder_arr(t, t->root, arr, i);
  return 0;
}
