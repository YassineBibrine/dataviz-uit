# Code Generator & Parser - Example Use Cases

## Example 1: Generate Array Code

### Visual Structure
```
Array: [64, 34, 25, 12, 22, 11, 90]
```

### Generated Code
```cpp
#include <iostream>
#include <vector>

int main() {
    // Initialize array
    std::vector<int> arr = {64, 34, 25, 12, 22, 11, 90};

    // Display array
    std::cout << "Array: ";
    for (int val : arr) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

### Usage
1. Select array structure
2. Generate code
3. Copy to IDE
4. Run to verify
5. Use for bubble sort implementation

---

## Example 2: Parse Linked List

### Input Code
```cpp
struct Node {
    int value;
    Node* next;
    Node(int val) : value(val), next(nullptr) {}
};

int main() {
    Node* head = new Node(10);
    Node* current = head;
    
    current->next = new Node(20);
    current = current->next;
    current->next = new Node(30);
    current = current->next;
    current->next = new Node(40);
    
    return 0;
}
```

### Parse Result
```
? Parse Successful!

Detected Type: Linked List

Values: 10, 20, 30, 40

? Structure created successfully!
ID: custom_1
```

### What Happens
- Structure appears in Structure Selector: "Parsed List [List] ?"
- Visualization shows: 10 ? 20 ? 30 ? 40 ? nullptr
- Toolbox switches to List mode (rectangular nodes)
- Ready for algorithm execution

---

## Example 3: Binary Tree Traversal

### Visual Tree
```
      50
  /  \
   30    70
   / \   / \
  20 40 60 80
```

### Generated Code
```cpp
#include <iostream>

struct TreeNode {
    int value;
 TreeNode* left;
    TreeNode* right;
    TreeNode(int val) : value(val), left(nullptr), right(nullptr) {}
};

TreeNode* insert(TreeNode* root, int value) {
    if (root == nullptr) {
     return new TreeNode(value);
    }
    if (value < root->value) {
        root->left = insert(root->left, value);
    } else {
    root->right = insert(root->right, value);
    }
    return root;
}

void inorderTraversal(TreeNode* root) {
    if (root == nullptr) return;
    inorderTraversal(root->left);
    std::cout << root->value << " ";
    inorderTraversal(root->right);
}

int main() {
    // Create binary search tree
    TreeNode* root = nullptr;

    // Insert nodes
    root = insert(root, 50);
    root = insert(root, 30);
    root = insert(root, 70);
    root = insert(root, 20);
    root = insert(root, 40);
    root = insert(root, 60);
    root = insert(root, 80);

    // Display tree (in-order)
    std::cout << "Binary Tree (in-order): ";
    inorderTraversal(root);
    std::cout << std::endl;

    return 0;
}
```

### Output
```
Binary Tree (in-order): 20 30 40 50 60 70 80
```

---

## Example 4: Parse Graph for BFS

### Input Code
```cpp
class Graph {
    // ... (implementation)
};

int main() {
  Graph g(false);  // undirected
    
  // Add nodes
    g.addNode("A");
    g.addNode("B");
    g.addNode("C");
    g.addNode("D");
  g.addNode("E");
    
    // Add edges
    g.addEdge("A", "B");
    g.addEdge("A", "C");
    g.addEdge("B", "D");
    g.addEdge("C", "D");
    g.addEdge("D", "E");
    
    return 0;
}
```

### Parse Result
```
? Parse Successful!

Detected Type: Graph

Nodes:
  A (value: 0)
  B (value: 1)
  C (value: 2)
  D (value: 3)
  E (value: 4)

Edges:
  A -> B
  A -> C
  B -> D
  C -> D
  D -> E

? Structure created successfully!
ID: custom_2
```

### Next Steps
1. Graph is visualized with circular layout
2. Select BFS algorithm
3. Choose start node (A)
4. Click Play
5. Watch BFS traverse: A ? B ? C ? D ? E

---

## Example 5: Debug Graph Structure

### Problem
You have buggy graph code with wrong connections.

### Buggy Code
```cpp
Graph g(false);
g.addNode("A");
g.addNode("B");
g.addNode("C");
g.addNode("D");

// Bug: Missing edge B->C, wrong edge A->D
g.addEdge("A", "B");
g.addEdge("A", "D");  // Should be B->C
g.addEdge("C", "D");
```

### Debug Process
1. **Parse the code**
   - Structure created from buggy code
   
2. **Visualize**
   ```
   A --- B C --- D
  \____________/
   ```
   - Immediately see A?D connection is wrong
   - Missing B?C edge visible
   
3. **Fix**
   - Identify issue visually
   - Correct: Remove A?D, add B?C
   
4. **Verify**
   - Re-parse corrected code
   - Confirm correct structure

### Fixed Code
```cpp
g.addEdge("A", "B");
g.addEdge("B", "C");  // Fixed
g.addEdge("C", "D");
```

### Fixed Visualization
```
A --- B --- C --- D
```

---

## Example 6: Student Homework Helper

### Assignment
"Implement a binary search tree and perform in-order traversal"

### Student Workflow

**Step 1: Visual Design**
- Create tree interactively
- Drag nodes, set values
- Connect with links

**Step 2: Generate Code**
- Click "Generate C++ Code"
- Get complete BST implementation

**Step 3: Understand**
- Study generated insert() function
- Understand traversal logic
- See memory management

**Step 4: Customize**
- Modify for specific requirements
- Add preorder/postorder
- Implement additional features

**Step 5: Submit**
- Well-structured, working code
- Visual proof of correctness
- Understanding of implementation

---

## Example 7: Algorithm Testing

### Scenario
Test QuickSort on different array configurations.

### Workflow

**Test Case 1: Random**
```cpp
std::vector<int> arr = {64, 34, 25, 12, 22, 11, 90};
```
- Parse ? Run QuickSort ? Verify

**Test Case 2: Already Sorted**
```cpp
std::vector<int> arr = {1, 2, 3, 4, 5, 6, 7};
```
- Parse ? Run QuickSort ? Check performance

**Test Case 3: Reverse Sorted**
```cpp
std::vector<int> arr = {7, 6, 5, 4, 3, 2, 1};
```
- Parse ? Run QuickSort ? Worst case?

**Test Case 4: Duplicates**
```cpp
std::vector<int> arr = {5, 2, 8, 2, 9, 1, 5};
```
- Parse ? Run QuickSort ? Handle duplicates

### Benefits
- Quick test case creation
- Visual comparison
- Performance analysis
- Edge case verification

---

## Example 8: Teaching Tool

### Instructor Use Case

**Lecture: "How Linked Lists Work"**

**Before Lecture:**
1. Create sample linked lists
2. Generate code for each
3. Prepare slides with visuals + code

**During Lecture:**
1. Show visual representation
2. Explain node connections
3. Display generated code side-by-side
4. Students see structure + implementation

**Live Demo:**
1. Write linked list code on board
2. Paste into parser
3. Instant visualization
4. "See? This is what your code creates!"

**Student Exercise:**
1. Give students code snippets
2. Students parse to visualize
3. Compare structures
4. Discuss implementations

---

## Example 9: Code Review

### Scenario
Review colleague's graph implementation.

### Original Code (Confusing)
```cpp
// Complex, hard-to-follow graph code
g.addNode("n1");
g.addNode("n2");
g.addNode("n3");
g.addEdge("n1", "n2");
g.addEdge("n2", "n3");
g.addEdge("n3", "n1");
// ... 50 more lines
```

### Review Process
1. **Parse Code**
   - Instant visualization
   - See actual structure

2. **Identify Issues**
   - "Wait, there's a cycle here!"
   - "This node is isolated"
   - "Unexpected edge found"

3. **Discuss**
   - Point to specific visual elements
   - Explain concerns clearly
   - Suggest improvements

4. **Generate Better Code**
   - Modify structure visually
   - Generate cleaner implementation
   - Share with team

---

## Example 10: Documentation

### Task
Document graph algorithms for wiki.

### Process

**Step 1: Create Examples**
- Build sample graphs visually
- Various sizes and complexities

**Step 2: Generate Code**
- Complete, runnable examples
- Include comments

**Step 3: Write Documentation**
```markdown
### Example Graph

![Graph Visualization](graph.png)

#### Implementation
```cpp
Graph g(false);
g.addNode("A");
g.addNode("B");
// ... generated code
```

#### Usage
1. Copy code above
2. Compile and run
3. Observe output
```

**Step 4: Maintain**
- Update visuals
- Regenerate code
- Keep docs in sync

---

## Summary

### Code Generator Best For:
? Learning implementations  
? Creating code templates  
? Documentation examples  
? Sharing structures as code  
? Understanding structure internals  

### Code Parser Best For:
? Debugging structures  
? Visualizing existing code  
? Quick prototyping  
? Testing algorithms  
? Code review visualization  

### Combined Power:
```
Idea ? Code ? Parse ? Visualize ? Modify ? Generate ? Refine
```

**Perfect for:**
- Education (learning + teaching)
- Development (prototyping + debugging)
- Collaboration (sharing + reviewing)
- Documentation (examples + tutorials)

---

**End of Examples**

For full documentation, see: `CODE_GENERATOR_GUIDE.md`
