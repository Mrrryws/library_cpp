#include "library.h"

#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

//***********************************************NOVEL***********************************************
// Constructor
Novel :: Novel(const char *titleIn, const char *authorIn, int yearIn, int quantityIn){
    _year = yearIn;
    _quantity = quantityIn;

    _author = (char*)malloc((strlen(authorIn) + 1) * sizeof(char));
    strcpy(_author, authorIn);

    _title = (char*)malloc((strlen(titleIn) + 1) * sizeof(char));
    strcpy(_title, titleIn);
}

// Destructor
Novel ::~Novel(){
    delete[] _author;
    delete[] _title;
}

// Getter for document type
document_type Novel :: getDocType(){
    return document_type :: DOC_NOVEL;
}

// Print function
void Novel :: print(){
    printf("Novel, title: %s, author: %s, year: %d, quantity: %d\n", getTitle(), getAuthor(), getYear(), getQuantity());
}

// Updater
void Novel :: updateAuthor(const char *newAuthor){
    delete[] _author;
    _author = (char*)malloc((strlen(newAuthor) + 1) * sizeof(char));
    strcpy(_author, newAuthor);
}

// Getter
char *Novel :: getAuthor(){
    return _author;
}

//***********************************************COMIC***********************************************

// Constructor
Comic :: Comic(const char *titleIn, const char *authorIn, int issueIn, int yearIn, int quantityIn){
    _issue = issueIn;
    _year = yearIn;
    _quantity = quantityIn;

    _author = (char*)malloc((strlen(authorIn) + 1) * sizeof(char));
    strcpy(_author, authorIn);

    _title = (char*)malloc((strlen(titleIn) + 1) * sizeof(char));
    strcpy(_title, titleIn);
}

// Destructor
Comic ::~Comic(){
    delete[] _author;
    delete[] _title;
}

// Getter for document type
document_type Comic :: getDocType(){
    return document_type :: DOC_COMIC;
}

// Print function
void Comic :: print(){
    printf("Comic, title: %s, author: %s, issue: %d, year: %d, quantity: %d\n", getTitle(), getAuthor(), getIssue(), getYear(), getQuantity());
}

// Updaters
void Comic :: updateAuthor(const char *newAuthor){
    delete[] _author;
    _author = (char*)malloc((strlen(newAuthor) + 1) * sizeof(char));
    strcpy(_author, newAuthor);
}

void Comic :: updateIssue(int newIssue){
    _issue = newIssue;
}

// Getters
char *Comic :: getAuthor(){
    return _author;
}

int Comic :: getIssue(){
    return _issue;
}

//***********************************************MAGAZINE***********************************************

// Constructor
Magazine :: Magazine(const char *titleIn, int issueIn, int yearIn, int quantityIn){
    _issue = issueIn;
    _year = yearIn;
    _quantity = quantityIn;

    _title = (char*)malloc((strlen(titleIn) + 1) * sizeof(char));
    strcpy(_title, titleIn);
}

// Destructor
Magazine ::~Magazine(){
    delete[] _title;
}

// Getter for document type
document_type Magazine :: getDocType(){
    return document_type :: DOC_MAGAZINE;
}

// Print function
void Magazine :: print(){
    printf("Magazine, title: %s, issue: %d, year: %d, quantity: %d\n", getTitle(), getIssue(), getYear(), getQuantity());
}

// Updater
void Magazine :: updateIssue(int newIssue){
    _issue = newIssue;
}

// Getter
int Magazine :: getIssue(){
    return _issue;
}

//***********************************************DOCUMENT***********************************************

// Updaters
void Document :: updateTitle(const char *newTitle){
    delete[] _title;
    _title = (char*)malloc((strlen(newTitle) + 1) * sizeof(char));
    strcpy(_title, newTitle);
}

void Document :: updateYear(int newYear){
    _year = newYear;
}

void Document :: updateQuantity(int newQuantity){
    _quantity = newQuantity;
}

// Getters
char *Document :: getTitle(){
    return _title;
}

int Document :: getYear(){
    return _year;
}

int Document :: getQuantity(){
    return _quantity;
}

// Borrow & Return documents
int Document :: borrowDoc(){
    if(_quantity >= 1){
        _quantity--;
        return 0;
    }
    return 1;
}

void Document :: returnDoc(){
    _quantity++;
}

//***********************************************LIBRARY***********************************************

Library :: Library(){

}

void Library :: print(){
    for(auto i = _docs.begin(); i!=_docs.end(); i++)
        (*i)->print();
}

int Library::dumpCSV(const char *filename) {
    int file_descriptor = open(filename, O_WRONLY | O_CREAT);
    if(file_descriptor == -1)
        return -1;

    for(Document *doc : _docs) {
        char c[80];
        document_type type = doc->getDocType();
        if(type == DOC_COMIC) {
            Comic *comic = (Comic *) doc;
            sprintf(c, "comic,%s,%s,%d,%d,%d\n", comic->getTitle(), comic->getAuthor(), comic->getIssue(), comic->getYear(), comic->getQuantity());
        } else if(type == DOC_MAGAZINE) {
            Magazine *magazine = (Magazine *) doc;
            sprintf(c, "magazine,%s,,%d,%d,%d\n", magazine->getTitle(), magazine->getIssue(), magazine->getYear(), magazine->getQuantity());
        } else if(type == DOC_NOVEL) {
            Novel *novel = (Novel *) doc;
            sprintf(c, "novel,%s,%s,,%d,%d\n", novel->getTitle(), novel->getAuthor(), novel->getYear(), novel->getQuantity());
        }
        if(!write(file_descriptor, c, strlen(c)))
            return -1;
    }

    close(file_descriptor);
    return 0;
}

Document *Library :: searchDocument(const char *title){
    vector<Document *> :: iterator i;
    for(i = _docs.begin(); i!=_docs.end(); i++)
        if(strcmp((*i)->getTitle(), title)==0){
            return (*i);
        }
    return NULL;
}

int Library :: addDocument(Document *d){
  vector<Document *> :: iterator i;
  for (i = _docs.begin(); i!= _docs.end(); i++)
    if(strcmp ((*i) -> getTitle(), d -> getTitle())==0)
      return 1;
  _docs.push_back(d);
  return 0;
}

int Library::delDocument(const char *title){
  vector<Document *> :: iterator i;
  for (i = _docs.begin(); i != _docs.end(); i++){
       if(!strcmp(title, (*i)->getTitle())){
         _docs.erase(i);
         return 0;
       }
   }
   return 1;
}

int Library :: countDocumentOfType(document_type t){
    int n=0;
    vector<Document *> :: iterator i;
    for(i = _docs.begin(); i!=_docs.end(); i++){
        if((*i) -> getDocType() ==t)
            n++;
    }
    return n;
}

int Library :: returnDoc(const char *title){
    vector<Document *> :: iterator i;
    for(i = _docs.begin(); i!=_docs.end(); i++)
        if(strcmp((*i)->getTitle(), title)==0){
            (*i)->returnDoc();
            return 0;
        }
    return 1;

}

int Library::borrowDoc(const char *title){
  vector<Document *> :: iterator i;
  for (i = _docs.begin(); i != _docs.end(); i++)
   {
       if(!strcmp(title, (*i)->getTitle())){
         if(!(*i)->borrowDoc()){
         (*i)->updateQuantity((*i)->getQuantity());
         return 0;
       }
       break;
      }
   }
   return 1;
}
