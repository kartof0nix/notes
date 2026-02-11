/* File copy program. Error checking and reporting is minimal. */
#include <sys/types.h> /* include necessary header files */
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstddef>
#include <iterator>

#include <bits/stdc++.h>
#include "fat_fs.cpp"

using namespace std;


#define OUTPUT_MODE 0644 /* protection bits for output file */

int main(int argc, char *argv[]); /* ANSI prototype */

fat_fs::Filesystem fsystem("test.fs");

vector<byte> read_pth(string path){
    vector<byte> res;
    if(path[0] == '@'){
        res = fsystem.fread(path.substr(1));
    }
    else{
        res = read_file(path);
    }
    return res;
}

bool write_pth(string path, byte *ptr, int n){
    if(path[0] == '@'){
        return fsystem.fwrite(path.substr(1), ptr, n);
    }
    else{
        return write_file(path, ptr, n);
    }
    return false;
    
}

bool remove_pth(string path){
    if(path[0] == '@'){
        return fsystem.remove(path.substr(1));
    }
    else{
        return false;
    }
    return false;

}

bool parse_cmd(string cmd){
    auto vs = split_sentence(cmd);
    if(vs.size() == 0) return false;
    if(vs[0] == "cp"){
        if(vs.size() != 3) return false;
        vector<byte> vb = read_pth(vs[1]);
        cout << vb.size() << " bytes loaded\n";
        cout << "Writing " << (write_pth(vs[2], &*vb.begin(), vb.size()) ? "OK" : "error") << "\n";
        return true;
    }
    if(vs[0] == "ls"){
        cout << fsystem << "\n";
        return true;
    }
    if(vs[0] == "rm"){
        return remove_pth(vs[1]);
    }
    return false;
}


int main(int argc, char *argv[])
{
    // int in_fd, out_fd, rd_count, wt_count;
    // if (argc != 2)
    //     exit(1); /* syntax error if argc is not 3 */ /* Open the input file and create the output file */
    //     fsystem.fs_name = argv[1];
    cerr << "Init Ok!\n";

    cout << fsystem << '\n';
    // fsystem.root.expand(100, -2);

    // // cout << "Free:" << fsystem.find_free() << '\n';
    // // fsystem.root.add_file(fsystem.find_free());
    // // fsystem.root.get_file(fsystem.root.n_dirs()-1)
    // cout << *fsystem.root.n_dirs() << '\n';
    // cout << fsystem << '\n';;


    // cout << "Path ''\n";
    // cout << *fsystem.get("");
    // cout << '\n';
    // cout << "Dir '/'\n";
    // cout << *fsystem.get_dir("");
    // cout << '\n';

    // cout << "Path '/'\n";
    // cout << *fsystem.get("/");
    // cout << '\n';
    // cout << "Dir '/'\n";
    // cout << *fsystem.get_dir("/");
    // cout << '\n';

    while(cin){
        cout << ">";
        string cmd;
        getline(cin, cmd);
        if(parse_cmd(cmd)){
            continue;
        }
        cout << "Wrong command\n";


        // cout << "Path:";
        // string fname;
        // cin >> fname;
        // auto f = fsystem.get(fname);
        // cout << *f;
        // if(f->get_type() == fat_fs::FileType::DIR){
        //     cout << "file is a directory, can't write directly!\n";
        // }
        // // char cur[f->size()];
        // auto r = fsystem.fread(f.get());
        // string res = "";
        // for(byte c : r) res += char(c);
        // cout << "Current :'" << res << "'\n";
        // // cout << "Current" << 
        // cout << "Text:";
        // string t; cin >> t;
        // fsystem.fwrite(fname, (byte*)(t.c_str()), t.size());

    }

    // cout << "Path '/'\n";
    // cout << *fsystem.root.get_path("/") << '\n';
    // cout << "Path '/noname'\n";
    // cout << *fsystem.root.get_path("/noname") << '\n';
    // fsystem.save_buf();
    // auto it = fat_fs::BufIterator(reinterpret_cast<byte*>(fsystem.buffer), fat_fs::addr_t(0));
    // while(it != fat_fs::BufIterator()){
    //     cout << char( *(it++)) << '|';
    // }
    // cout << '\n';
    
    // fsystem.read_buf();
    // cout << fsystem << '\n';

    // int bytes = fsystem.read_buf();
    // strcpy(fsystem.buffer+bytes, "\nKolejna linijka\n");
    fsystem.save_buf();
}