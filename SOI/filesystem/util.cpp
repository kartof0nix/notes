#include <sys/types.h> /* include necessary header files */
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstddef>
#include <iterator>

#include <bits/stdc++.h>

using namespace std;

#define OUTPUT_MODE 0644 /* protection bits for output file */


vector<string> split_sentence(string sen) {
  
    // Create a stringstream object
    stringstream ss(sen);
    
    // Variable to hold each word
    string word;
    
    // Vector to store the words
    vector<string> words;
    
    // Extract words from the sentence
    while (ss >> word) {
      
        // Add the word to the vector
        words.push_back(word);
    }
    
    return words;
}

template<class IT>
bool write_file(string path, IT ptr, int n){
    int out_fd, wt_count;
    byte buf[n+1];
    copy_n(ptr, n, buf);

    out_fd = creat(path.c_str(), OUTPUT_MODE); /* create the destination file */
    wt_count = write(out_fd, buf, n); /* write data */
    if (wt_count <= 0)
        exit(4); /* wt_count <= 0 is an error */
    close(out_fd);   /* if end of file or error, exit loop */
    return true;
}

constexpr int BUF_SIZE = 1024 * 16; // 16 KB
vector<byte> read_file(string path){
    int in_fd, rd_count;
    in_fd = open(path.c_str(), O_RDONLY); /* open the source file */
    if (in_fd < 0){
        cerr << "Filesystem file not present!\n";
        close(in_fd);
        return {};
    }
    vector<byte> bf(BUF_SIZE);
    rd_count = read(in_fd, &*(bf.begin()), bf.size()); /* read a block of data */
    close(in_fd);
    bf.resize(rd_count);
    return bf;
}