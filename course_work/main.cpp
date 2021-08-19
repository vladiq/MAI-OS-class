#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <chrono>

#define COMMANDS 8192

class Page {
public:
	int R;
	int M;
	int count;
	int page;
	long fetchTime;

	Page(int page = -1) {
		R = 0;
		M = 0;
		count = 0;
		this->page = page;
		this->fetchTime = 0;
	}
};

class PageNode {
public:
	PageNode(PageNode* next = nullptr) {
		this->page = new Page();
		this->next = next;
	}

	~PageNode() { 
		delete page; 
	}

	Page* page;
	PageNode* next;
};

class PageList {
public:
	int size;
	PageNode* head; 
	PageNode* tail;

	PageList(int size) {
		this->size = size;
		head = tail = new PageNode();
		PageNode* curr = head;
		for (int i = 1; i < size; i++) {
			curr = curr->next = new PageNode();
		}
	}

	~PageList() {
		PageNode* temp;
		while (head->next != nullptr) {
			temp = head;
			head = head->next;
			delete temp;
		}
		delete head;
	}

	PageNode* findPage(int& page) {
		PageNode* curr = head;
		while (curr != nullptr) {
			if (curr->page->page == page) {
				return curr;
			}
			curr = curr->next;
		}
		return nullptr;
	}
};

class PageReplacementAlgo {
public:
	virtual void ReferencePage(PageList& memory, PageNode* pageNode, int instruction) = 0;
	virtual PageNode* selectPage(PageList& memory) = 0;
	virtual std::string AlgoName() = 0;
};

class Aging : public PageReplacementAlgo {
public:
	void Refresh(PageList& memory) {
		PageNode* curr = memory.head;
		int interval;
		while (curr != nullptr) {
			interval = clock() - curr->page->fetchTime;
			curr->page->fetchTime = clock();
			curr->page->count >> interval;
			curr->page->count |= curr->page->R << 8;
			curr->page->R = 0;
			curr = curr->next;
		}
	}

	void ReferencePage(PageList& memory, PageNode* pageNode, int instruction) {
		Refresh(memory);
		pageNode->page->R = 1;
		pageNode->page->count++;
	}

	PageNode* selectPage(PageList& memory) {
		if (memory.tail->next != nullptr || memory.tail->page->fetchTime == 0) {
			PageNode* temp = memory.tail;
			if (memory.tail->next != nullptr) {
				 memory.tail = memory.tail->next;
			}
			return temp;
		}
		PageNode* selectPage = memory.head;
		PageNode* temp = selectPage->next;
		while (temp != nullptr) {
			if (temp->page->count < selectPage->page->count) { 
				selectPage = temp;
			}
			temp = temp->next;
		}
		return selectPage;
	}

	std::string AlgoName() { 
		return "Aging"; 
	}
};

class NRU : public PageReplacementAlgo {
public:
	void Refresh(PageList& memory) {
		PageNode* curr = memory.head;
		long currTime = clock();
		while (curr != nullptr && currTime - curr->page->fetchTime > 20) { 
			curr->page->R = 0;
			curr = curr->next;
		}
	}

	void ReferencePage(PageList& memory, PageNode* pageNode, int instruction ) {
		pageNode->page->R = 1;
		if (instruction % 10 == 1) {
			pageNode->page->M = 1;
		}
	}

	PageNode* selectPage(PageList& memory) {
		Refresh(memory);
		if (memory.tail->next != nullptr || memory.tail->page->fetchTime == 0) {
			PageNode* temp = memory.tail;
			if (memory.tail->next != nullptr) { 
				memory.tail = memory.tail->next;
			}
			return temp;
		}

		PageNode* selectPage = memory.head;
		PageNode* temp = selectPage->next;
		while (temp != nullptr) {
			int value1 = temp->page->R * 2 + temp->page->M;
			int value2 = selectPage->page->R * 2 + selectPage->page->M;
			if (value1 < value2) {
				selectPage = temp;
			}
			temp = temp->next;
		}
		return selectPage;
	}

	std::string AlgoName() {
		return "NRU"; 
	}
};

void fetchPage(PageNode* pageNode, int page) {
	pageNode->page->page = page;
	pageNode->page->R = 0;
	pageNode->page->M = 0;
	pageNode->page->count = 0;
	pageNode->page->fetchTime = clock();
}

void execute(PageReplacementAlgo* algo, PageList& memory, int address, int instruction, int& hit, int& miss ) {
	int page = address / 10;
	PageNode* pageNode = memory.findPage(page);
	if (pageNode != nullptr) {
		algo->ReferencePage(memory, pageNode, address);
		hit++;
	} else {
		PageNode* pageNode = algo->selectPage(memory);
		fetchPage(pageNode, page);
		algo->ReferencePage(memory, pageNode, address);
		miss++;
	}
}

void MemDispatch(PageReplacementAlgo& f, int physicalMemory, int instructions[]) {
	PageList memory(physicalMemory);
	int instr_pos;

	int hit = 0, miss = 0;
	srand(10);
	for (int i = 0; i < COMMANDS; i += 4) {
		instr_pos = rand() % (COMMANDS - 1);
		execute(&f, memory, instr_pos + 1, instructions[instr_pos + 1], hit, miss);
		instr_pos = rand() % instr_pos;
		execute(&f, memory, instr_pos, instructions[instr_pos], hit, miss);
		execute(&f, memory, instr_pos + 1, instructions[instr_pos + 1], hit, miss);
		instr_pos = rand() % (COMMANDS - 2 - instr_pos) + instr_pos + 2;
		execute(&f, memory, instr_pos, instructions[instr_pos], hit, miss);
	}
	std::cout << "Page hits: " << hit << "\t page faults: " << miss << "\t hit rate : " << 1 - (double) miss / COMMANDS << std::endl;
}

int main() {
	PageReplacementAlgo* algo[2];
	Aging aging;
	algo[0] = &aging;
	NRU nru;
	algo[1] = &nru;

	srand(time(nullptr));
	int instructions[COMMANDS];
	for (int i = 0; i < COMMANDS; i += 1) {
		instructions[i] = 10 * rand() / 65535 + 228;
	}

	std::chrono::time_point<std::chrono::system_clock> startNRU, startAging, endNRU, endAging;

	startNRU = std::chrono::system_clock::now();
	for (int j = 4; j <= 1024; j += 32) {
		std::cout << "Memory pool: " << j << std::endl;
		std::cout << algo[1]->AlgoName() << "\t";
		MemDispatch(nru, j, instructions);
	}
	endNRU = std::chrono::system_clock::now();

	std::cout << "\n\n\n";

	startAging = std::chrono::system_clock::now();
	for (int j = 4; j <= 1024; j += 32) {
		std::cout << "Memory pool: " << j << std::endl;
		std::cout << algo[0]->AlgoName() << "\t";
		MemDispatch(aging, j, instructions);
	}
	endAging = std::chrono::system_clock::now();

	int NRUTime = std::chrono::duration_cast<std::chrono::seconds> (endNRU - startNRU).count();
	int AgingTime = std::chrono::duration_cast<std::chrono::seconds> (endAging - startAging).count();

	std::cout << "NRU Time: " << NRUTime << std::endl << "Aging time: " << AgingTime << std::endl;
	return 0;
} 
