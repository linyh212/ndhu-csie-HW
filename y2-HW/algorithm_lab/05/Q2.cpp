#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <algorithm>

// ##### STUDENT'S IMPLEMENTATION AREA START #####
// ##### 學生的實作區域 START #####

// Suggested Node structure for the Huffman tree
// 建議的霍夫曼樹節點結構
struct HuffmanNode {
    HuffmanNode *left_child, *right_child;
    int frequency;
    char character; // Use a special character (e.g., '\0' or a negative int if char becomes int) for internal nodes
                    // 對於內部節點，使用特殊字元（例如 '\0'，或者如果字元類型改為 int，則使用負數）

    // Constructor for leaf nodes // 葉節點建構子
    HuffmanNode(int freq, char ch)
        : frequency(freq), character(ch), left_child(nullptr), right_child(nullptr) {}

    // Constructor for internal nodes // 內部節點建構子
    HuffmanNode(int freq, HuffmanNode* left, HuffmanNode* right)
        : frequency(freq), character('\0'), left_child(left), right_child(right) {} // Assuming '\0' for internal nodes

    // Destructor to free tree memory (important!)
    // 解構子以釋放樹記憶體 (重要!)
    ~HuffmanNode() {
        delete left_child;
        delete right_child;
    }
};

// Comparator for the priority queue (to make it a min-heap based on frequency)
// 優先佇列的比較器 (使其成為基於頻率的最小堆積)
struct CompareHuffmanNodes {
    bool operator()(const HuffmanNode* a, const HuffmanNode* b) const {
        // Nodes with higher frequency should have lower priority (for min-heap)
        // 頻率較高的節點應具有較低的優先級 (對於最小堆積)
        if (a->frequency != b->frequency) {
            return a->frequency > b->frequency;
        }
        // Optional: Tie-breaking for consistent tree structure.
        // E.g., if frequencies are equal, prioritize node with smaller character (if both are leaves)
        // or by some other consistent rule (like node creation time, though harder to track).
        // 可選：決勝規則以確保一致的樹結構。
        // 例如，如果頻率相等，則優先考慮字元較小的節點 (如果兩者都是葉節點)，
        // 或依其他一致規則 (如節點創建時間，雖然較難追蹤)。
        if (a->character != '\0' && b->character != '\0') { // Both are leaves
            return a->character > b->character;
        }
        // If one is internal, other is leaf, or both internal, a simple pointer comparison (not portable)
        // or just no further tie-breaking might be acceptable for this problem if not strictly required.
        return false; // Default behavior if frequencies are equal and no further tie-breaking
    }
};

// Helper function: Recursive traversal to generate codes and store them in codes_map
// 輔助函式：遞迴遍歷以產生編碼並將其儲存於 codes_map
void populate_codes_recursive(const HuffmanNode* node, std::string current_huffman_code, std::map<char, std::string>& codes_map) {
    // TODO: Students might implement this helper function.
    // TODO: 學生可能需要實作此輔助函式。
    // Base case: if the node is null, return.
    // 基本情況：如果節點為空，則返回。
    if(!node) return;
    // If it's a leaf node (identified by character != '\0' or by having no children),
    // add its character and current_huffman_code to codes_map.
    // Handle the special case for a single-character text (code should be "0").
    // 如果是葉節點 (透過 character != '\0' 或沒有子節點來識別)，
    // 將其字元和 current_huffman_code 加入 codes_map。
    // 處理單一字元文本的特殊情況 (編碼應為 "0")。
    if(!node->left_child && !node->right_child){
        if(node->frequency==0) codes_map[node->character]="0";
        else codes_map[node->character]=current_huffman_code;
        return;
    }
    // Recursively call for left child (append "0" to current_huffman_code)
    // and right child (append "1" to current_huffman_code).
    // 遞迴呼叫左子節點 (將 "0" 附加到 current_huffman_code)
    // 和右子節點 (將 "1" 附加到 current_huffman_code)。
    populate_codes_recursive(node->left_child, current_huffman_code+"0", codes_map);
    populate_codes_recursive(node->right_child, current_huffman_code+"1", codes_map);
}


std::map<char, std::string> generate_huffman_codes(const std::string& text) {
    std::map<char, std::string> codes_map;
    if (text.empty()) {
        return codes_map;
    }

    // TODO: Implement the Huffman coding algorithm.
    // TODO: 實作霍夫曼編碼演算法。
    // 1. Count character frequencies and store them in a map.
    //    計算字元頻率並將其儲存於 map 中。
    std::map<char, int> frequency_map;
    for(char ch: text){
        frequency_map[ch]++;
    }
    // 2. Create a leaf node (HuffmanNode) for each character with its frequency.
    //    Push these nodes into a min-priority queue (std::priority_queue with CompareHuffmanNodes).
    //    為每個具有其頻率的字元建立一個葉節點 (HuffmanNode)。
    //    將這些節點推入最小優先佇列 (使用 CompareHuffmanNodes 的 std::priority_queue)。
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, CompareHuffmanNodes> min_heap;
    for(auto const& [ch, freq]: frequency_map){
        min_heap.push(new HuffmanNode(freq, ch));
    }
    // 3. While the priority queue has more than one node:
    //    a. Extract the two nodes with the smallest frequencies (let's call them left and right).
    //    b. Create a new internal node with frequency equal to the sum of left's and right's frequencies.
    //       Make left and right the children of this new node.
    //    c. Insert the new internal node back into the priority queue.
    //    當優先佇列中的節點超過一個時：
    //    a. 提取頻率最小的兩個節點 (稱之為 left 和 right)。
    //    b. 建立一個新的內部節點，其頻率等於 left 和 right 頻率之和。
    //       使 left 和 right 成為此新節點的子節點。
    //    c. 將新的內部節點插回優先佇列。
    while(min_heap.size()>1){
        HuffmanNode* left=min_heap.top();
        min_heap.pop();
        HuffmanNode* right=min_heap.top();
        min_heap.pop();
        HuffmanNode* new_node=new HuffmanNode(left->frequency+right->frequency, left, right);
        min_heap.push(new_node);    
    }
    // 4. The last node remaining in the priority queue is the root of the Huffman tree.
    //    優先佇列中最後剩下的節點是霍夫曼樹的根。
    HuffmanNode* root=min_heap.top();
    // 5. Traverse the Huffman tree (e.g., using a recursive helper function like populate_codes_recursive)
    //    to generate the Huffman codes for each character and store them in codes_map.
    //    Remember to handle the case of a single unique character in the text (its code should be "0").
    //    遍歷霍夫曼樹 (例如，使用類似 populate_codes_recursive 的遞迴輔助函式)
    //    以產生每個字元的霍夫曼編碼並將其儲存於 codes_map 中。
    //    記得處理文本中只有一個唯一字元的情況 (其編碼應為 "0")。
    if (root != nullptr) {
        populate_codes_recursive(root, "", codes_map);
        delete root; // Clean up memory
    }
    // 6. Important: Clean up dynamically allocated memory for HuffmanNodes after codes are generated.
    //    The destructor in HuffmanNode should handle recursive deletion if called on the root.
    //    重要：產生編碼後，清理為 HuffmanNode 動態分配的記憶體。
    //    如果在根節點上呼叫，HuffmanNode 中的解構子應處理遞迴刪除。


    // Example structure for step 5, assuming 'root' is the Huffman tree root:
    // 步驟 5 的範例結構，假設 'root' 是霍夫曼樹的根：
    // if (root != nullptr) {
    //    populate_codes_recursive(root, "", codes_map);
    //    delete root; // Clean up memory
    // }

    return codes_map;
}
// ##### STUDENT'S IMPLEMENTATION AREA END #####
// ##### 學生的實作區域 END #####
int main() {
    using namespace std;
    string text1;
    std::cin>>text1;
    map<char, string> codes1 = generate_huffman_codes(text1);
    cout << "Huffman codes for '" << text1 << "':" << endl;
    for (auto const& [key, val] : codes1) {
        cout << key << ": " << val << endl;
    }
    return 0;
}
