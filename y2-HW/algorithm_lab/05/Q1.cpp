// ##### STUDENT'S IMPLEMENTATION AREA START #####
// ##### 學生的實作區域 START #####
unsigned long long calculate_encoded_length(const std::string& text, const HuffmanCode& huffman_encoder) {
    // TODO: Implement this function.
    // TODO: 實作此函式。
    // Iterate through `text`. For each character, get its Huffman code string from `huffman_encoder`
    // and add the length of that code string to a running total.
    // 遍歷 `text`。對每個字元，從 `huffman_encoder` 取得其霍夫曼編碼字串，
    // 並將該編碼字串的長度累加到總長度中。
    unsigned long long total_length = 0;
    // Your code here // 你的程式碼寫在這裡
    for(char c : text){
        std::string code=huffman_encoder.get_code(c);
        total_length+=code.length();
    }
    return total_length;
}
// ##### STUDENT'S IMPLEMENTATION AREA END #####
// ##### 學生的實作區域 END #####
int main() {
    using namespace std;
    string text1;
    std::cin>>text1;
    HuffmanCode encoder1(text1);
    cout << "Huffman codes for '" << text1 << "':" << endl;
    for (auto const& [key, val] : encoder1.get_code_table()) {
        cout << key << ": " << val << endl;
    }
    unsigned long long length1 = calculate_encoded_length(text1, encoder1);
    cout << "Encoded length for '" << text1 << "': " << length1 << endl;
    return 0;
}
