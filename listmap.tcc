// $Id: listmap.tcc,v 1.15 2019-10-30 12:44:53-07 - - $

#include "listmap.h"
#include "debug.h"

//
/////////////////////////////////////////////////////////////////
// Operations on listmap.
/////////////////////////////////////////////////////////////////
//

//
// listmap::~listmap()
//
template <typename key_t, typename mapped_t, class less_t>
listmap<key_t,mapped_t,less_t>::~listmap() {
   DEBUGF ('l', reinterpret_cast<const void*> (this));
   while(!empty()){
     erase(begin());
   }
}

//
// iterator listmap::insert (const value_type&)
//
template <typename key_t, typename mapped_t, class less_t>
typename listmap<key_t,mapped_t,less_t>::iterator
listmap<key_t,mapped_t,less_t>::insert (const value_type& pair) {
   DEBUGF ('l', &pair << "->" << pair);
   if(empty()){
     node* N= new node(anchor(),anchor(),pair);
     anchor_.next=N;
     anchor_.prev=N;
     return iterator(N);
   }
   listmap<key_t,mapped_t,less_t>::iterator iter=begin();
   while(iter!=end()){
     if(!(less(pair.first, iter->first))&& !(less(iter->first,pair.first))){
       iter->second=pair.second;
       return iter;
     }
     if(less(pair.first, iter->first)){
       node* N= new node(iter.where, (iter.where)->prev, pair);
       ((iter.where)->prev)->next=N;
       iter.where->prev=N;
       return iterator(N);
     }
     ++iter;
   }
   node* N= new node(anchor(),anchor()->prev,pair);
   (anchor()->prev)->next=N;
   anchor()->prev=N;
   return iterator(N);
}

//
// listmap::find(const key_type&)
//
template <typename key_t, typename mapped_t, class less_t>
typename listmap<key_t,mapped_t,less_t>::iterator
listmap<key_t,mapped_t,less_t>::find (const key_type& that) {
   DEBUGF ('l', that);
   listmap<key_t,mapped_t,less_t>::iterator iter;
   for(iter=begin();
   iter!=end();++iter){
     if(!(less(that, iter->first))&& !(less(iter->first,that ))){
       return iter;
     }
   }
   return iter;
}

template <typename key_t, typename mapped_t, class less_t>
void listmap<key_t,mapped_t,less_t>:: find_val(const mapped_t& that){
  listmap<key_t,mapped_t,less_t>::iterator iter;
   for(iter=begin();
   iter!=end();++iter){
     if(!(less(that, iter->second))&& !(less(iter->second,that ))){
       cout<<*iter<<endl;
     }
   }
}

//
// iterator listmap::erase (iterator position)
//
template <typename key_t, typename mapped_t, class less_t>
typename listmap<key_t,mapped_t,less_t>::iterator
listmap<key_t,mapped_t,less_t>::erase (iterator position) {
   DEBUGF ('l', &*position);
   listmap<key_t,mapped_t,less_t>::iterator iter=position;
   ++iter;
   node* D=position.where;
   (D->prev)->next=D->next;
   (D->next)->prev=D->prev;
   delete D;
   return iter;
}


