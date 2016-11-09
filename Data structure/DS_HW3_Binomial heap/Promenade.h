#include<iostream>
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<climits>
using namespace std;



class Person{
public:
	Person();
	Person(int i, int e);
	~Person();

private:
	Person *parent, *child, *sibling; // the pointers to his parent, child, sibling
	int id; // record Person's id
	int energy; // record Person's energy
	int degree;// record how many levels of this Person's children
	friend class Promenade;//you could access all valuables in class Promenade
};

//ctor
Person::Person(){
	parent = child = sibling = NULL;
	id = 0;
	energy = 0;
	degree = 0;
}

//ctor
Person::Person(int i, int e){// define a new Person with id = i and energy = e
	parent = child = sibling = NULL;
	id = i;
	energy = e;
	degree = 0;
}

//dtor
Person::~Person(){

}

class Promenade{

public:
	Promenade();
	~Promenade();
	void one_person_joined(int id, int energy);
	void one_group_joined(Promenade *g);
	void absorb_energy(int id, int energy);
	int  calculate_people_below(int energy);
	int  get_weakest_person();
	int  size();

private:
	Person *head;
	int num;//caculate how many people in the promenade
};

//ctor
Promenade::Promenade(){
	head = NULL;
	num = 0;
}

//dtor
Promenade::~Promenade(){

}

void Promenade::one_person_joined(int id, int energy){
	//TODO: fill in the code to simulate a person with his id and energy to join the Promenade
	//cout<<"energy++"<<endl;

	if(head==NULL) //empty binomial heap
	{
		Person* rootperson=new Person();
		rootperson->id=id;
		rootperson->energy=energy;
		head=rootperson;
		return;
	}
	    Person *newperson=new Person(); //insert a new person
	    newperson->id=id;
		newperson->energy=energy;
		Person *heap = NULL, *pre_x = NULL, *x = NULL, *(next_x) = NULL,
		*firstHeap = NULL, *secondHeap = NULL,
        *pre_H3 = NULL,*H3 = NULL;
		
		if (head != NULL && newperson != NULL){
	    firstHeap = head;
        secondHeap = newperson;
	 

		while (firstHeap != NULL && secondHeap != NULL) {
            if (firstHeap->degree <= secondHeap->degree) {
                H3 = firstHeap;
                firstHeap = firstHeap->sibling;
            } else {
                H3 = secondHeap;
                secondHeap = secondHeap->sibling;
            }
 
            if (pre_H3 == NULL) {
                pre_H3 = H3;
                heap = H3;
            } else {
                pre_H3->sibling = H3;
                pre_H3 = H3;
            }
            if (firstHeap != NULL) {
                H3->sibling = firstHeap;
            } else {
                H3->sibling = secondHeap;
            }
        }//while
	}else if (head != NULL) {
        heap = head;
    } else {
        heap = newperson;
    }
    head= newperson = NULL;
	head=heap;
    
	
	// Here we have got the increasing sorted heap
	
	if (heap == NULL) {   
	   return ;
    }
	
	pre_x = NULL;
    x = heap;
    next_x = x->sibling;
	while (next_x != NULL) {
        if ((x->degree != next_x->degree) ||//Cases 1 and 2
            ((next_x->sibling != NULL) && (next_x->degree == next_x->sibling->degree))) {
                pre_x = x;
                x = next_x;
        } else if (x->energy <= next_x->energy) {//Cases 3
            x->sibling = next_x->sibling;
            
			//Make x be the parent of next_x
			next_x->parent = x;
            next_x->sibling = x->child;
            x->child = next_x;
            x->degree++;
			
        } else {//Cases 4
            if (pre_x == NULL) {
                heap = next_x;
            } else {
                pre_x->sibling = next_x;
            }
			
			//Make next_x be the parent of x
           
            x->parent = next_x;
            x->sibling = next_x->child;
            next_x->child = x;
            next_x->degree++;
			
			x = next_x;
        }
        next_x = x->sibling;
    }
	
	
	head=heap;
	 
}

void Promenade::one_group_joined(Promenade *g){
	//TODO: fill in the code to simulate one group to join the full promenade
     
	 Person *heap = NULL, *pre_x = NULL, *x = NULL, *(next_x) = NULL,
		*firstHeap = NULL, *secondHeap = NULL,
        *pre_H3 = NULL,*H3 = NULL;
	   
	   
	   if (head != NULL && g->head != NULL){
	   firstHeap = head;
       secondHeap = g->head;
	   


        while (firstHeap != NULL && secondHeap != NULL) {
            if (firstHeap->degree <= secondHeap->degree) {
                H3 = firstHeap;
                firstHeap = firstHeap->sibling;
            } else {
                H3 = secondHeap;
                secondHeap = secondHeap->sibling;
            }
 
            if (pre_H3 == NULL) {
                pre_H3 = H3;
                heap = H3;
            } else {
                pre_H3->sibling = H3;
                pre_H3 = H3;
            }
            if (firstHeap != NULL) {
                H3->sibling = firstHeap;
            } else {
                H3->sibling = secondHeap;
            }
        }//while
	   }else if (head != NULL) {
        heap = head;
    } else {
        heap = g->head;
    }
    head= g->head = NULL;
	head=heap;
	
   
	
	// Here we have got the increasing sorted heap
	
	if (heap == NULL) {
        return ;
    }
	
	pre_x = NULL;
    x = heap;
    next_x = x->sibling;
	
	while (next_x != NULL) {
        if ((x->degree != next_x->degree) ||//Cases 1 and 2
            ((next_x->sibling != NULL) && (next_x->degree == next_x->sibling->degree))) {
                pre_x = x;
                x = next_x;
        } else if (x->energy <= next_x->energy) {//Cases 3
            x->sibling = next_x->sibling;
            
			//Make x be the parent of next_x
			next_x->parent = x;
            next_x->sibling = x->child;
            x->child = next_x;
            x->degree++;
			
        } else {//Cases 4
            if (pre_x == NULL) {
                heap = next_x;
            } else {
                pre_x->sibling = next_x;
            }
			
			//Make next_x be the parent of x
           
            x->parent = next_x;
            x->sibling = next_x->child;
            next_x->child = x;
            next_x->degree++;
			
			x = next_x;
        }
        next_x = x->sibling;
    }
	head=heap;
	
}

void Promenade::absorb_energy(int id, int energy){
	//TODO: Elizabeth absord the energy of the person with this id, this Person's energy will decrease by this number.
	//		If his energy becomes lower than or equal to zero, he will die and should be thrown out automatically

   if(head==NULL)
	{
		return;
	}
	
	//There is only one person in the heap
	if(head->sibling==NULL&&head->child==NULL)
	{
		if(head->id==id)
		{
			if(head->energy<=energy)//delete head
			{
			head=NULL;
			return;
			}
			else{
			head->energy=head->energy-energy;
		    return;
			}
		}
		else return;
	}
	
	//More than one person in the heap
	
	Person *find_it=NULL;
	Person *heap1=head;
	Person *heap2=head;
	
	
	while(heap2->sibling!=NULL)
	{
		heap2=heap2->sibling;
	}
	
	
	// the rightest sibling root among the root list is in heap2
	
	while(heap1!=heap2)
	{
		
	while(heap1->child!=NULL)
	 {
		if(heap1->id==id)
		{
			find_it=heap1;
            			
	    }
		heap1=heap1->child;
	 }
	    if(heap1->id==id)
		{
			find_it=heap1; 
			
	    }
	if(heap1->sibling!=NULL)
	 {
		heap1=heap1->sibling;
	 }
	else{
		heap1=heap1->parent->sibling;
	}
	}
      	//move to the child of the rightest root
		if(heap1->child!=NULL)
		{
			if(heap1->id==id)
		  {
			find_it=heap1;
		  }
			heap1=heap1->child;
		}
		
		//now we have come to the rightest root
	while(!((heap1->sibling==NULL)&&(heap1->parent->sibling==NULL)))
	{
		if(find_it!=NULL)
			break;
	  while(heap1->child!=NULL)
	  {
	    if(heap1->id==id)
		{
			find_it=heap1;
		    
	    }
        heap1=heap1->child;	  
	  }
	    if(heap1->id==id)
		{
			find_it=heap1;
		    
	    }
	  if(heap1->sibling!=NULL)
	    {
		heap1=heap1->sibling;
	    }
	  else{
		heap1=heap1->parent->sibling;
	      }
	
	}
	if(heap1->id==id)
		{
			find_it=heap1;
	    }
		
	if(find_it==NULL)
	    return;
	
	
	// we have find this person
	
	//case one: we just delete this Person
	if(find_it->energy<=energy)
	{
		//we firstly decrease it's energy
		find_it->energy=INT_MIN;
		Person *x=NULL,
		       *y=NULL;
	    x = find_it;
        y = find_it->parent;
    while(y != NULL && y->energy > x->energy) {
        swap(y->energy, x->energy);
		swap(y->id,x->id);
        x = y;
        y = x->parent;
    }  
	   //minimum is in the root x;
	 
	   //then we delete the min
	   Person *pre_y = NULL, *y1 = NULL, *x2 = head;
       int min = INT_MAX;
		
		while (x2 != NULL) {
        if (x2->energy < min) {
            min = x2->energy;
            pre_y = y1;
            y1 = x2;
        }
        
        x2 = x2->sibling;
        
		}
 
        //min is in y1
    if (y1 == NULL) {
        return;
    }
      
    if (pre_y == NULL) {
        head = head->sibling;
    } else {
		
		while(pre_y->sibling!=y1)
	  {
		  pre_y=pre_y->sibling;
	  }
        pre_y->sibling = y1->sibling;
    }
 
    //reverse pointer of the child node of y1
    Person *H2 = NULL, *p = NULL;
    x2 = y1->child;
    while (x2 != NULL) {
        p = x2;
        x2 = x2->sibling;
        p->sibling = H2;
        H2 = p;
        p->parent = NULL;
    }
 
    //---------------------------------------------------
	
	//then we union head and H2
	
	Person *heap = NULL, *pre_x = NULL, *(next_x) = NULL,
		*firstHeap = NULL, *secondHeap = NULL,
        *pre_H3 = NULL,*H3 = NULL;
	   
	   
	   if (head != NULL && H2 != NULL){
	   firstHeap = head;
       secondHeap = H2;
	   
        while (firstHeap != NULL && secondHeap != NULL) {
            if (firstHeap->degree <= secondHeap->degree) {
                H3 = firstHeap;
                firstHeap = firstHeap->sibling;
            } else {
                H3 = secondHeap;
                secondHeap = secondHeap->sibling;
            }
 
            if (pre_H3 == NULL) {
                pre_H3 = H3;
                heap = H3;
            } else {
                pre_H3->sibling = H3;
                pre_H3 = H3;
            }
            if (firstHeap != NULL) {
                H3->sibling = firstHeap;
            } else {
                H3->sibling = secondHeap;
            }
        }//while
	   }else if (head != NULL) {
        heap = head;
    } else {
        heap = H2;
    }
    head= H2 = NULL;
	head=heap;
   
	
	// Here we have got the increasing sorted heap
	if (heap == NULL) {
        return ;
    }
	
	pre_x = NULL;
    x = heap;
    next_x = x->sibling;
	
	while (next_x != NULL) {
        if ((x->degree != next_x->degree) ||//Cases 1 and 2
            ((next_x->sibling != NULL) && (next_x->degree == next_x->sibling->degree))) {
                pre_x = x;
                x = next_x;
        } else if (x->energy <= next_x->energy) {//Cases 3
            x->sibling = next_x->sibling;
            
			//Make x be the parent of next_x
			next_x->parent = x;
            next_x->sibling = x->child;
            x->child = next_x;
            x->degree++;
			
        } else {//Cases 4
            if (pre_x == NULL) {
                heap = next_x;
            } else {
                pre_x->sibling = next_x;
            }
			
			//Make next_x be the parent of x
           
            x->parent = next_x;
            x->sibling = next_x->child;
            next_x->child = x;
            next_x->degree++;
			
			x = next_x;
        }
        next_x = x->sibling;
    }
	head=heap;
	
	}
	//case two: we decrease it's energy
	else 
	{
		find_it->energy=(find_it->energy-energy); //decrease
		
		Person *w = NULL, *q = NULL;
         q = find_it;
         w = find_it->parent;
    while(w != NULL && w->energy > q->energy) {
        swap(w->energy, q->energy);
		swap(w->id,q->id);
        q = w;
        w = q->parent;
         }	
	}
}

int Promenade::calculate_people_below(int energy){
	//TODO: You should calculate how many people's energy are below the number (include the number) and return it
	
	int count=0;
	if(head==NULL)
	{return 0;}
    
	if(head->sibling==NULL&&head->child==NULL)
	{
		if(head->energy<=energy)
		{
			count++;
			return count;
		}
		else{
			return 0;
		}
	}
	
	Person *heap1=head;
	Person *heap2=head;
	
	while(heap2->sibling!=NULL)
	{
		heap2=heap2->sibling;
	}
	
	
	// the rightest sibling among the root list is in heap2
	
	while(heap1!=heap2)
	{
		
	while(heap1->child!=NULL)
	{
		if(heap1->energy<=energy)
			{count++;}
		heap1=heap1->child;
	}
	    if(heap1->energy<=energy)
			{count++;}
	if(heap1->sibling!=NULL)
	{
		heap1=heap1->sibling;
	}
	else{
		heap1=heap1->parent->sibling;
	}
	}
  
		if(heap1->child!=NULL)
		{
			if(heap1->energy<=energy)
			{count++;}
			heap1=heap1->child;
		}
		
		//now heap1 is the B0 in the head
	while(!((heap1->sibling==NULL)&&(heap1->parent->sibling==NULL)))
	{
	  while(heap1->child!=NULL)
	  {
	        if(heap1->energy<=energy)
			{count++;}
        heap1=heap1->child;	  
	  }
	        if(heap1->energy<=energy)
			{count++;}
	  if(heap1->sibling!=NULL)
	    {
		heap1=heap1->sibling;
	    }
	  else{
		heap1=heap1->parent->sibling;
	      }
	
	}
	        if(heap1->energy<=energy)
			{count++;}
	
	return count;
}

int Promenade::get_weakest_person(){
	//TODO: return the id of the one with the least energy left. If there are more than one solution, just return one of them
    if(head==NULL)
	{
		cout<<"There is no person in the heap,";
	   return -9999;
	}
	if(head->sibling==NULL&&head->child==NULL)
	{
		
			return head->id;
		
	}
	Person *head1=head;
	while(head1!=NULL)
	{
		head1=head1->sibling;
	}
	int weakest=head->id;
	Person *heap1=head;
	Person *heap2=head;
	while(heap1)
	{
		if(heap1->energy<heap2->energy)
		{
			heap2=heap1;
		}
		heap1=heap1->sibling;
	}
	weakest=heap2->id;
	return weakest;
}

int Promenade::size(){
	
	int num=0;
	
	if(head==NULL)
	{return 0;}
    
	if(head->sibling==NULL&&head->child==NULL)
	{
		//	cout<<"id:"<<head->id<<"\t"<<"energy:"<<head->energy<<endl;
			return 1;
	}
	Person *heap1=head;
	Person *heap2=head;
	
	while(heap2->sibling!=NULL)
	{
		heap2=heap2->sibling;
	}
	
	
	// the rightest sibling among the root list is in heap2
	
	while(heap1!=heap2)
	{
		
	while(heap1->child!=NULL)
	{
		num++;
		//cout<<"id:"<<heap1->id<<"\t"<<"energy:"<<heap1->energy<<endl;
		heap1=heap1->child;
	}
	    num++;
	//	cout<<"id:"<<heap1->id<<"\t"<<"energy:"<<heap1->energy<<endl;
	if(heap1->sibling!=NULL)
	{
		heap1=heap1->sibling;
	}
	else{
		heap1=heap1->parent->sibling;
	}
	}
    // cout<<"----------------------------"<<endl;	 	
		if(heap1->child!=NULL)
		{
			num++;
			//cout<<"id:"<<heap1->id<<"\t"<<"energy:"<<heap1->energy<<endl;
			heap1=heap1->child;
		}
	
		
		//now heap1 is the B0 in the heap
	while(!((heap1->sibling==NULL)&&(heap1->parent->sibling==NULL)))
	{
	  while(heap1->child!=NULL)
	  {
	    num++;
		//cout<<"id:"<<heap1->id<<"\t"<<"energy:"<<heap1->energy<<endl;
        heap1=heap1->child;	  
	  }
	    num++;
		//cout<<"id:"<<heap1->id<<"\t"<<"energy:"<<heap1->energy<<endl;
	  if(heap1->sibling!=NULL)
	    {
		heap1=heap1->sibling;
	    }
	  else{
		heap1=heap1->parent->sibling;
	      }
	
	}
	num++;
	//cout<<"id:"<<heap1->id<<"\t"<<"energy:"<<heap1->energy<<endl;
	return num;
}
