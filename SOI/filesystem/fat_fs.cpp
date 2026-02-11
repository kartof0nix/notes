/* File copy program. Error checking and reporting is minimal. */
#include <sys/types.h> /* include necessary header files */
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstddef>
#include <iterator>
#include <cstddef>
#include "util.cpp"
#include <bits/stdc++.h>

using namespace std;


#define OUTPUT_MODE 0644 /* protection bits for output file */

int main(int argc, char *argv[]); /* ANSI prototype */

// typedef unsigned char byte;

namespace fat_fs{
    typedef short addr_t;

    constexpr addr_t ADDRES_BYTES = sizeof(addr_t);
    constexpr addr_t FTYPE_BYTES = 1;
    constexpr addr_t NAME_BYTES = 31;

    // constexpr addr_t FAT_ENTRY_BYTES = ADDRES_BYTES + NAME_BYTES;
    
    // constexpr addr_t MAX_FILES = 10;
    // constexpr addr_t FAT_BYTES = MAX_FILES * FAT_ENTRY_BYTES;
    constexpr addr_t FAT_BYTES = 0;
    
    constexpr addr_t BLOCK_DATA_BYTES = 126; // 30 BYTES = DATA;
    constexpr addr_t BLOCK_BYTES = ADDRES_BYTES + BLOCK_DATA_BYTES; // 2 Bytes = NEXT_ADDR, 30 BYTES = DATA;
    constexpr addr_t BUF_BLOCKS = 100;

    constexpr addr_t BREG_BYTES = BLOCK_BYTES * BUF_BLOCKS;

    constexpr addr_t BUF_SIZE = 0 + BREG_BYTES;

    /**
     * @short Abstract buffer iterator (reads byte by byte)
     */
    class BufIterator{

        byte *buf;
        addr_t byte_it;  // Number of currently read 
        public:
        addr_t block_it; // Number of currently read block
        using difference_type = std::ptrdiff_t;
        using value_type = std::byte;
        using reference = std::byte&;
        using pointer = std::byte*;
        BufIterator(){
            block_it = -1;
            byte_it = 0;
        };
        BufIterator(const BufIterator& other){
            buf = other.buf;
            block_it = other.block_it;
            byte_it = other.byte_it;
        };
        BufIterator(byte *buffer, addr_t block){
            buf = buffer;
            block_it = block;
            byte_it = 0;
        };
        bool operator==(const BufIterator& other) const{
            return (block_it == other.block_it) && (byte_it == other.byte_it);
        }
    
        BufIterator& operator=(const BufIterator& other){
            buf = other.buf;
            block_it = other.block_it;
            byte_it = other.byte_it;
            return *this;
        }
        byte& operator*() const{
            assert(byte_it < BLOCK_DATA_BYTES); // Within the byte
            if(next_block() == 0){
                throw overflow_error("Overflow at" + to_string(block_it) + ", " + to_string(byte_it));
            }
            assert(next_block() != 0); // next_block = 0 <=> Byte is free but we are reading
            return *(buf + FAT_BYTES + block_it * (BLOCK_BYTES) + ADDRES_BYTES + byte_it);
        };
        short next_block() const{
            return *(reinterpret_cast<short*>(buf + FAT_BYTES + block_it * (BLOCK_BYTES)));
        }
        void set_next_block(short block){
            *(reinterpret_cast<short*>(buf + block_it * (BLOCK_BYTES))) = block;
        }
        BufIterator& operator+=(addr_t it){
            assert(block_it != -1);
            byte_it+= it;
            while(byte_it >= BLOCK_DATA_BYTES){
                byte_it -= BLOCK_DATA_BYTES;
                if(next_block() == -1)
                    throw out_of_range("Invalid pointer: " + to_string(block_it));
                if(next_block() == addr_t(-1)){

                    byte_it = 0;
                    block_it = -1;
                    return *this;
                }
                byte_it = 0;
                block_it = next_block();
            }
            return *this;
        };
        BufIterator operator+(addr_t it){
            BufIterator c = *this;
            c += it;
            return c;
        };
        BufIterator& operator++(){
            return operator+=(1);
        };
        BufIterator operator++(int)
        {
            auto tmp = *this;
            ++*this;
            return tmp;
        }
        byte* raw_ptr() const{
            return (buf + FAT_BYTES + block_it * (BLOCK_BYTES) + ADDRES_BYTES + byte_it);
        }
    };
    static_assert(std::forward_iterator<BufIterator>);

    enum FileType{
        NONE = 0,
        DIR = 1,
        FILE = 2
    };

    class nofile_error : public std::exception {
    private:
        std::string message;
    
    public:
        explicit nofile_error(const std::string& path)
            : message("No such file: " + path) {}
    
        const char* what() const noexcept override {
            return message.c_str();
        }
    };

    struct File{

    protected:
        byte *buffer;
        short BLOCK_0;

        BufIterator _raw_start() const{
            return BufIterator(buffer, BLOCK_0);
        }

    public:
        File(byte *buf_ptr, const short block_0) : buffer(buf_ptr), BLOCK_0(block_0) {
            if(!exists())
                expand(BLOCK_0);
        };
        File() : buffer(NULL), BLOCK_0(0){};

        // void operator=(const File &f){
        //     BLOCK_0 = f.BLOCK_0;
        //     buffer = f.buffer;
        // }
        bool exists() const{
            return buffer[BLOCK_0 * BLOCK_BYTES] != byte(0);
        }
        FileType get_type() const{
            return FileType(*_raw_start());
        }
        void set_type(FileType type){
            *_raw_start() = byte(type);
        }

        char* get_name() const{
            return reinterpret_cast<char*> (&(*(_raw_start() + FTYPE_BYTES )));
        }
        void set_name(string name){
            assert(name.size() <= NAME_BYTES);
            char* ptr = reinterpret_cast<char*> (&(*(_raw_start() + FTYPE_BYTES )));
            strcpy(ptr, name.c_str());
        }
        BufIterator start() const {
            return (_raw_start() + FTYPE_BYTES + NAME_BYTES);
        }
        BufIterator end() const{
            return BufIterator();
        }
        short blocks() const{
            BufIterator it = _raw_start();
            short res = it.next_block() != 0;
            while(it.next_block() != -1){
                res++;
                it += BLOCK_DATA_BYTES;
            }
            return res;
        }
        addr_t size() const{
            return BLOCK_DATA_BYTES * blocks() - (FTYPE_BYTES + NAME_BYTES);
        }
        void expand(short free_block){
            BufIterator it = _raw_start();
            if(it.next_block() == 0){
                it.set_next_block(-1);
            }
            addr_t cur_bytes = BLOCK_DATA_BYTES;
            while(it.next_block() != -1){
                if(cur_bytes > BLOCK_DATA_BYTES * BUF_BLOCKS){
                    throw logic_error("Buffer iterator self loop");
                }
                it += BLOCK_DATA_BYTES;
                cur_bytes += BLOCK_DATA_BYTES;
            }
            if(free_block == it.block_it) return;
            it.set_next_block(free_block);
            cerr << it.block_it << ' ' << it.next_block() << '\n';

            it += BLOCK_DATA_BYTES;

            it.set_next_block(-1);
            cerr << it.block_it << ' ' << it.next_block() << '\n';
            cerr << _raw_start().next_block() << "\n";
        }
        void shrink(addr_t bytes){
            BufIterator it = _raw_start();
            if(it.next_block() == 0){
                it.set_next_block(-1);
            }
            addr_t cur_bytes = BLOCK_DATA_BYTES;
            while(it.next_block() != -1 || cur_bytes < bytes){
                it += BLOCK_DATA_BYTES;
                cur_bytes += BLOCK_DATA_BYTES;
            }
            if(it == _raw_start() && bytes == 0){
                it.set_next_block(0);
                return;
            }
            if(cur_bytes <= bytes) return;
            BufIterator next_it = it + BLOCK_DATA_BYTES;
            if(it.next_block() == -1) return;
            it.set_next_block(-1);
            swap(next_it, it);
            while(it.next_block() != -1){
                BufIterator next_it = it + BLOCK_DATA_BYTES;
                it.set_next_block(0);
                swap(next_it, it);
            }
            it.set_next_block(0);
        }

        ostream &desc(ostream &out) const{
            
            if(exists()){
                out << "[" << get_type()   << "]";
            }
            else{
                out<<"[x]";
            }
            out << "(" << BLOCK_0 << ") ";
            out << "{" << blocks() << "} ";
            if(exists())
                out << " " << get_name() << "" ;
            return out << '\n';

        }
        virtual std::unique_ptr<File> get_path(string path) const{
            if(path == "") return std::make_unique<File>(File(*this));
            return NULL;
        }
        virtual ostream &print(ostream &out, const int level = 0) const {
            string prefix = "";
            for(int i = 0; i < level; i++) prefix += " ";
            prefix += "|-";
            out << prefix;
            return desc(out);
        }

        // void set_start(addr_t new_start){
        //     *(reinterpret_cast<addr_t*> (buffer + ENTRY_OFFSET + NAME_BYTES)) = new_start;
        // }
        // void delete_entry(){
        //     set_name("");
        // }
    };

    struct Directory : public File{
        Directory(byte *buf_ptr, const short block_0) : File(buf_ptr, block_0) {
        };
        Directory() : File() {};
        void init(){
            set_type(FileType::DIR);
        }
        short* n_dirs() const{
            return (reinterpret_cast<short*> (start().raw_ptr()));
        }
        File add_file(short block){
            ++*n_dirs();
            auto f = File(buffer, block);
            // f.expand(block);
            f.set_name("noname");
            *get_file_block(*n_dirs()-1) = block;
            return f;
        }

        
        
        short* get_file_block(short file) const{
            return reinterpret_cast<short*> ((start() + ADDRES_BYTES * (file+1)).raw_ptr());
        }
        
        
        
        std::unique_ptr<File> get_file(int i) const{

            if(i >= *n_dirs()) {
                throw invalid_argument("File not in talbe");
                return std::make_unique<File>(Directory(*this));
            }
            File f = File(buffer, *get_file_block(i));
            if(f.get_type() == FileType::DIR){
                // preserve dynamic type: return a Directory held by unique_ptr<File>
                return std::unique_ptr<File>(new Directory(buffer, *get_file_block(i)));
            }
            return std::unique_ptr<File>(new File(f));
        }


        void rem_file(int i){
            if(i >= *n_dirs()) {
                throw invalid_argument("File not in talbe");
            }
            auto rm_file = get_file_block(i);
            auto last_file = get_file_block(*n_dirs()-1);
            *rm_file = 0;
            swap(*rm_file, *last_file);
            *n_dirs() = *n_dirs() - 1;
        }
        void rem_file(string fname){
            rem_file(get_file_idx(fname));
        }
        
        short get_file_idx(string fname) const{
            for(short i = 0; i < *n_dirs(); i++){
                std::unique_ptr<File> f = get_file(i);
                if( f->get_name() == fname){
                    return i;
                }
            }
            throw nofile_error(fname);
        }
        std::unique_ptr<File> get_path(string path) const override{
            if(path[0] == '/') path = path.substr(1);
            if(path == "") return std::unique_ptr<File>(new Directory(buffer, BLOCK_0));
            // Above line slices Directory -> File; change to:
            // if(path == "") return std::unique_ptr<File>(new Directory(*this));
            int slash_id = path.size();
            for(int i = 0; i < path.size(); i++){
                if(path[i] == '/'){
                    slash_id = i;
                    break;
                }
            }
            string fname = path.substr(0, slash_id);
            string rem = (slash_id == path.size() ? "" : path.substr(slash_id + 1));
            short i = get_file_idx(fname);
            return get_file(i)->get_path(rem);
            return NULL;
        }

        ostream &print(ostream &out, const int level = 0) const override{
            if(level>50){
                throw logic_error("Filesystem self-loop");
            }
            string prefix = "";
            for(int i = 0; i < level; i++) prefix += " ";
            prefix += "|-D";
            out << prefix;
            desc(out);
            for(int i = 0; i < *n_dirs(); i++){
                auto p = get_file(i);
                p->print(out, level+1);
                // out << '\n';
            }
            return out;
        }

    };
    ostream &operator<<(ostream &out, const File &f)
    {
        return f.print(out);
    }

    ostream &operator<<(ostream &out, const Directory &f)
    {
        return f.print(out);
    }

    struct Filesystem{
        /*
            Blocks marked by:
                (addr_t)(-1) = last byte
                (addr_t)( 0) = free block
                (addr_t)( x) = x is next block

        
        */
        
    private:
        
        byte buffer[BUF_SIZE];
        
        string fs_name;
    public:
        Directory root;
        bool save_buf(){
            int out_fd, wt_count;

            out_fd = creat(fs_name.c_str(), OUTPUT_MODE); /* create the destination file */
            wt_count = write(out_fd, buffer, BUF_SIZE); /* write data */
            if (wt_count <= 0)
                exit(4); /* wt_count <= 0 is an error */
            close(out_fd);   /* if end of file or error, exit loop */
            return true;
        }

        int read_buf(){
            int in_fd, rd_count;
            in_fd = open(fs_name.c_str(), O_RDONLY); /* open the source file */
            if (in_fd < 0){
                cerr << "Filesystem file not present!\n";
                close(in_fd);
                return false;
            }
            rd_count = read(in_fd, buffer, BUF_SIZE); /* read a block of data */
            close(in_fd);
            return rd_count;
        }

        Filesystem(string filesystem_file) : fs_name(filesystem_file), root(buffer, 0){
            read_buf();
            root.init();
        }
    private:
        /**
         * @return Iterator of first free byte block
         */
        short find_free(){
            for(short i = 1; i < BUF_BLOCKS; i++){
                if( buffer[ FAT_BYTES + i * BLOCK_BYTES] == byte(0)) return i;
            }
            return -1;
        }
        
    public:
        bool resize(File *file, addr_t bytes){
            if(file->size() - BLOCK_DATA_BYTES >= bytes){
                file->shrink(bytes);
            }
            while(file->size() < bytes){
                short b = find_free();
                if(b == -1) return false;
                file->expand(b);
            }
            return true;
        }
        std::unique_ptr<File> get(string path){
            return root.get_path(path);
        }
        std::unique_ptr<Directory> get_dir(string path){
            std::unique_ptr<Directory> res;
            {
                std::unique_ptr<File> f = get(path);
                if (Directory *d = dynamic_cast<Directory*> (f.get()); d != nullptr)
                {
                    res = std::unique_ptr<Directory>(new Directory(*d));
                }
                else{
                    throw nofile_error("No such directory");
                }
            }
            return res;
        
        }
        bool fwrite(File *f, byte* ptr, int n){
            try{
                if(not resize(f, n+1)) return false;
                cout << n << " - " << f->size() <<'\n';
                std::copy_n(ptr, n, f->start());
                return true;
            }
            catch(const invalid_argument &e){
                cerr << e.what() << '\n';
                return false;
            }
        }
        bool fwrite(string path, byte* ptr, int n){
            try{
                unique_ptr<File> f;
                try{
                    unique_ptr<File> f = get(path);
                } 
                catch(nofile_error){
                    string dir = path.substr(0, path.find_last_of('/'));
                    string fname = path.substr(path.find_last_of('/')+1);
                    if (dir == path){
                        dir = "";
                        fname = path;
                    }
                    auto d = get_dir(dir);
                    if(d == nullptr) return false;
                    f = std::make_unique<File>(d->add_file(find_free()));
                    f->set_name(fname);
                }
                return fwrite(f.get(), ptr, n);
            }
            catch(const exception &e){
                cerr <<"\n!Exception : " << e.what() << '\n';

                return false;
            }
        }
        vector<byte> fread(File *f){
            try{
                int n = f->size();
                vector<byte> res(n);
                cout << n << '\n';
                std::copy_n(f->start(), n-1, res.begin());
                return res;
            }
            catch(const exception &e){
                return {};
            }
        }
        vector<byte> fread(string path){
            try{
                auto f = get(path);
                if(f == nullptr) return {};

                return fread(f.get());
            }
            catch(const exception &e){
                cerr << e.what() << '\n';
                return {};
            }
        }
        bool remove(string path){
            try{
                string dir = path.substr(0, path.find_last_of('/'));
                string fname = path.substr(path.find_last_of('/')+1);
                if (dir == path){
                    dir = "";
                    fname = path;
                }
                cout << "remove in " << dir << "\n";
                auto d = get_dir(dir);
                if(d == nullptr) return false;
                auto f = get(path);
                d->rem_file(fname);
                f->set_name("");
                f->shrink(0);
                return true;
            }
            catch(const std::exception &e){
                cerr << "Exception: " << e.what() << '\n';
                return false;
            }
            return false;
        }
    };

    // Overload << for output
    ostream &operator<<(ostream &out, const Filesystem &f)
    {
        out << "Filesystem:\n";
        out << f.root;
        return out;
    }
}
