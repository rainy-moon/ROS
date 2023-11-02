#include"ros.h"

void init_simlist(struct SIMLIST* sl){
	sl->size =  0;
	sl->head = (struct node*)NULL;
	return;
}

int simlist_insert(struct SIMLIST* sl,int index,struct node*newnode){
	if(index>sl->size) return 0;
	if(!newnode) return 0;
	//插在链表头
	struct node* p=sl->head;
	if(index==0){
		newnode->next=p;
		sl->head=newnode;
		sl->size++;
		return 1;
	}
	else{
		while(index--)
			p=p->next;
		newnode->next=p->next;
		p->next=newnode;
		sl->size++;
		return 1;
	}
}

int simlist_delete(struct SIMLIST* sl,int index){
	if(index>sl->size) return 0;
	struct node* p = sl->head;
	//删头
	if(index==0){
		sl->head = sl->head->next;
		sl->size--;
		if(!mem_free(p,sizeof(struct node))) return 0;
		return 1;
	}
	else {
		while(index--)
			p=p->next;
		struct node* temp = p->next;
		p->next = temp->next;
		sl->size--;
		if(!mem_free(temp,sizeof(struct node))) return 0;
		return 1;
	}
}

struct node* simlist_get(struct SIMLIST* sl,int index){
	if(index<0||index>=sl->size) return NULL;
	else{
		struct node* p = sl->head;
		while(index--)
			p=p->next;
		return p;
	}
}


int simlist_free(struct SIMLIST* sl){
	if(sl->size==0) return 0;
	struct node* p1 = sl->head;
	struct node* p2 = p1->next;
	for(int i=0;i<sl->size;i++){
		while(p1){
			if(!mem_free(p1,sizeof(struct node))) return 0;
			p1=p2;
			p2=p1->next;
		}
	}
	sl->size = 0;
	sl->head = (struct node*) NULL;
	return 1;
}

void simlist_sortedinsert(struct SIMLIST* sl,struct node* newnode,int index){
	struct node* p = sl->head;
	if(!p || p->val[index]>newnode->val[index]){
		//插头
		newnode->next=p;
		sl->head=newnode;
		sl->size++;
		return ;
	}
	while(p->next && p->next->val[index]<=newnode->val[index])
		p=p->next;
	newnode->next=p->next;
	p->next=newnode;
	sl->size++;
	return ;
}

int simlist_find(struct SIMLIST* sl,int value,int index){
	int index=0;
	struct node* n = sl->head;
	for(;index<sl->size;index++){
		if(n->val[index]==value) break;
		else n=n->next;
	}
	if(index>=sl->size) return -1;
	else return index;
}