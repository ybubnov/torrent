#include "bencode/type/element.h"

using namespace bencode;

int64_t BencodeElement::elements = 0;

BencodeElement::BencodeElement(){
    hash = elements;
    elements++;
}

void BencodeElement::gc(){
    elements = 0;
}

int BencodeElement::compare(BencodeElement* belement){
    return this->get_hash() - belement->get_hash();    
}

int BencodeElement::get_hash(){
    return (int)hash;
}
