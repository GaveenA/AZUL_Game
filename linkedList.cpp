#include "linkedList.h"

LinkedList::LinkedList()
{
    this->head=nullptr;
    this->tail = nullptr;
    this->size = 0;
}

LinkedList::~LinkedList() {
    clear();
}

int LinkedList::getSize() {
    return this->size;
}

Tile* LinkedList::get(int i) {
    int index = 0;
    Tile* returnTile = nullptr;
    if (i>=0 && i<this->getSize())
    {
        Node* current = this->head;
        if (this->head != nullptr)
        {
            while(index < i && current->next != nullptr)
            {
            index++;
            current = current->next;
            }
            returnTile = current->tile;
        }
        else{
            throw ("Head poinnting to Nullptr");
        }
    }
    else {
      throw std::out_of_range("Linked List get - index out of range");
    }
    return returnTile;
}

void LinkedList::addFront(Tile* t)
{
    Node* toAdd = new Node(t,nullptr,nullptr);
    if (this->getSize() == 0)
    {
        this->head=toAdd;
        this->tail=toAdd;
    }
    else
    {
        // Saving current Head;
        Node* temp = this->head;

        toAdd->next=temp;
        temp->prev = toAdd;
        this->head = toAdd;
    }
    this->size++;
}

void LinkedList::addBack(Tile* t)
{
    Node* toAdd = new Node(t,nullptr,nullptr);
    if (this->getSize() == 0)
    {
        this->head=toAdd;
        this->tail=toAdd;
    }
    else
    {
        // Saving current Tail;
        Node* temp = this->tail;

        toAdd->prev = temp;
        temp->next = toAdd;
        this->tail = toAdd;
    }
    this->size++;
}

void LinkedList::deleteAt(int i)
{
    Node* toDelete = nullptr;
    int index = 0;
    if (i>=0 && i<this->getSize())
    {
        Node* current = this->head;
        if (this->head != nullptr)
        {
            while(index < i && current->next != nullptr)
            {
            index++;
            current = current->next;
            }
            toDelete = current;
        }
        else{
            throw std::logic_error("Invalid Operation: Deleting Empty List");
        }
    }
    else {
      throw std::out_of_range("Linked List deleteAt(index) - index out of range");
    }

    if(i==0){
        if(this->size == 1) {
            this->head = nullptr;
        } else {
            this->head = toDelete->next;
            this->head->prev = nullptr;
        }

        delete toDelete;
    }
    else if(i == this->getSize()-1){
        this->tail = toDelete->prev;
        this->tail->next = nullptr;
        delete toDelete;
    }
    else{
        // Node to the Left of 'toDelete'
        Node* LeftOF_toDelete = toDelete->prev;

        // Node to the Rigth of 'toDelete'
        Node* RightOF_toDelete = toDelete->next;

        LeftOF_toDelete->next = RightOF_toDelete;
        RightOF_toDelete->prev = LeftOF_toDelete;
        delete toDelete;
    }
    this->size--;
}

void LinkedList::deleteFront() {
    if (this->getSize() == 0)
    {
        throw std::logic_error("Invalid Operation: Deleting Empty List");
    }
    else
    {
        // Saving Current Head
        Node* temp = this->head;

        if(this->getSize() == 1)
        {
            this->head = nullptr;
            this->tail = nullptr;
            delete temp;
        }
        else
        {
            // New Head
            this->head = temp->next;
            this->head->prev = nullptr;
            delete temp;
        }
        this->size--;
    }
}

void LinkedList::deleteBack() {
    if (this->getSize() == 0)
    {
        throw std::logic_error("Invalid Operation: Deleting Empty List");
    }
    else
    {
        // Saving Current Tail
        Node* temp = this->tail;

        if(this->getSize() == 1)
        {
            this->head = nullptr;
            this->tail = nullptr;
            delete temp;
        }
        else
        {
            this->tail = temp->prev;
            this->tail->next = nullptr;
            delete temp;
        }
        this->size--;
    }
}

void LinkedList::clear() {
    while(this->getSize() != 0)
    {
        deleteFront();
    }

}

void LinkedList::addAt(Tile* tile, int i)
{
    Node* toAdd = new Node(tile, nullptr, nullptr);
    int index = 0;
    Node* position = nullptr;
    // Finding node at which to add
    if (i>=0 && i<=this->getSize())
    {
        Node* current = this->head;
        if (this->head != nullptr)
        {
            while(index < i && current->next != nullptr)
            {
            index++;
            current = current->next;
            }
            position = current;
        }
    }
    else {
      throw std::out_of_range("Linked List addAt(index) - index out of range");
    }

    if (this->getSize() == 0)
    {
        this->head=toAdd;
        this->tail=toAdd;
    }
    else if(i==0)
    {
        // Saving current Head;
        Node* temp = this->head;
        toAdd->next=temp;
        temp->prev = toAdd;
        this->head = toAdd;
    }
    // else if(i == this->getSize()-1){
    //     Node* temp = this->tail;
    //     temp->next = toAdd;
    //     toAdd->prev = temp;
    //     toAdd->next = nullptr;
    //     this->tail = toAdd;
    // }
    else if(i == this->getSize()-1){
        Node* temp = this->tail->prev;
        temp->next = toAdd;
        toAdd->prev = temp;
        toAdd->next = this->tail;
        this->tail->prev = toAdd;
    }
    else if(i == this->getSize()){
        Node* temp = this->tail;
        temp->next = toAdd;
        toAdd->prev = temp;
        toAdd->next = nullptr;
        this->tail = toAdd;

    }
    else{
        // Node to the Left of 'toAdd' position = current
        Node* LeftOF_Pos = position->prev;

        // // Node to the Rigth of 'toDelete'
        // Node* RightOF_Pos = position->next;

        LeftOF_Pos->next = toAdd;
        toAdd->prev = LeftOF_Pos;

        toAdd->next = position; // Fitting toAdd between LeftOfPos and 'position'
        position->prev = toAdd;
    }
    this->size++;
}
