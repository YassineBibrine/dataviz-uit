#include "data_model_manager.h"
#include <sstream>
#include <cstdlib>

void* DataModelManager::createDataStructure(const std::string& type, int size) {
 return generateRandomData(type, size);
}

void* DataModelManager::generateRandomData(const std::string& type, int size) {
 if (type == "Array") {
 auto* arr = new std::vector<int>(); arr->reserve(size);
 for (int i=0;i<size;++i) arr->push_back(rand()%100);
 return arr;
 }
 if (type == "LinkedList") {
 head = new ListNode(0); ListNode* cur=head; for (int i=1;i<size;++i){ cur->next = new ListNode(i); cur=cur->next; }
 return head;
 }
 return nullptr;
}

std::string DataModelManager::serializeToDOT(void* data) {
 (void)data; std::ostringstream os; os << "digraph G {\n // TODO\n}\n"; return os.str();
}
