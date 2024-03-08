#include "main.h"

void list_init(list *list){
    list->head=NULL;
}

void* create_newnode(void* num){
  node *newnode=malloc(sizeof(node));
  newnode->data=num;
  newnode->next=NULL;
  return newnode;
}

void list_insertion(list *list,void *num){
  node *ptr=list->head;
  while(ptr!=NULL){
    ptr=ptr->next;
  }
  node *newnode=create_newnode(num);
  // if(list->head==NULL){
  //   list->head=newnode;
  // }else{
    node *current=list->head;
    node *previous=NULL;
    while(current!=NULL){
      //currentが削除したいnum、もしくはcurrentがNULLになった終了する
      previous=current;
      current=current->next;
    }
    if(previous==NULL){
      //previousがNULLっていうことはnewnode->dataが(list->head->data)よりも小さかったということ、もしくはlist->headがNULLだったということ
      newnode->next=list->head;
      list->head=newnode;
    }else{//
      previous->next=newnode;
      newnode->next=current;
      //listの一番最後だったとしても上のwhile()でcurrentはNULLの状態で終了しているので問題ないし、途中へのinsertだったとしても問題ない
    }
  //}
}

void list_deletion(list *list,void* num){
  if(list->head->data==num){
    node *temp=list->head;
    list->head=list->head->next;
    //free(temp);
    return;
  }else{
    node *current=list->head;
    node *previous=NULL;
    while(current!=NULL && current->data!=num){
      //currentが削除したいnum、もしくはcurrentがNULLになった終了する
      previous=current;
      current=current->next;
    }
    if(current==NULL){
      //currentがNULLになるまでwhile()が回ったということは削除したいnumが存在しなかったということ
      printf("!-- no exist %d in list --!\n",num);
    }else{
      //currentがNULLじゃないということは、削除したいnumを見つけたということ
      node *temp=current;
      previous->next=current->next;//currentがlistの一番最後だったとしても、current->nextはNULLなので問題ない
      //free(temp);
    }
  }
}
