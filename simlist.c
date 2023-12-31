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
		while(--index)
			p=p->next;
		newnode->next=p->next;
		p->next=newnode;
		sl->size++;
		return 1;
	}
}

struct node* simlist_delete(struct SIMLIST* sl,int index){
	if(index>sl->size) return 0;
	struct node* p = sl->head;
	//删头
	if(index==0){
		sl->head = sl->head->next;
		sl->size--;
		p->next=NULL;
		return p;
	}
	else {
		while(--index)
			p=p->next;
		struct node* temp = p->next;
		p->next = temp->next;
		sl->size--;
		temp->next=NULL;
		return temp;
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

void simlist_resort(struct SIMLIST* sl,int pos,int index){
	struct node* p = sl->head;
	if(!pos){
		//链表头重排
		if(!(p->next) || p->val[index]<p->next->val[index]);//头结点还是最小，不变
		else{
			struct node* q = p->next;
			while(q->next&&q->next->val[index]<=p->val[index])
				q=q->next;
			sl->head = p->next;
			p->next = q->next;
			q->next = p;
		}
	}
	else{
		while(--pos)
			p = p->next;
		struct node* q = p->next;
		p->next = q->next;
		sl->size--;
		simlist_sortedinsert(sl,q,index);
	}
}

int simlist_find(struct SIMLIST* sl,int value,int index){
	int i=0;
	struct node* n = sl->head;
	for(;i<sl->size;i++){
		if(n->val[index]==value) break;
		else n=n->next;
	}
	if(i>=sl->size) return -1;
	else return i;
}

struct node* nodecpy(struct node* n){
	struct node* temp = (struct node*) mem_malloc(sizeof(struct node));
	if(!temp);
	else{ 
		for(int i=0;i<254;i++)
			temp->val[i] = n->val[i];
		temp->next = NULL;
	}
	return temp;
	
}