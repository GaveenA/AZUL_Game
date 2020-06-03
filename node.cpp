#include "node.h"

Node::Node(Tile* tile, Node* next, Node* prev) {
    this->tile=tile;
    this->next = next;
    this->prev = prev;
}

Node::Node(Node& other)
{
    this->tile=other.tile;
    this->next=other.next;
    this->prev=other.prev;
}

Tile* Node::data() {
    return this->tile;
}

Node* Node::getNext() {
    return this->next;
}

Node* Node::getPrev() {
    return this->prev;
}
