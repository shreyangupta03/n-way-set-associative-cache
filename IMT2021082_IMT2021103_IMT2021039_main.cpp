#include <bits/stdc++.h>
using namespace std;
#define ll long long int 
#define sp " "

// structure of a cache block 
typedef struct block{
    bool valid;
    ll tag,time;
}block;

// this function extracts the index bits from the memory address 
ll getIndex(ll x,ll indexBits,ll blockOff){
    x=x>>blockOff;
    ll a=(ll)pow(2,indexBits)-1;
    return a&x;
}

// this function extracts the tag bits from the address 
ll getTag(ll x,ll indexBits,ll blockOff){
    ll a=indexBits+blockOff;
    x=x>>a;
    return x;
}

// this function does majority of the work
// it takes the input, modifies the cache accordingly, and reports whether a hit or a miss has occured  
bool run(ll addr,vector<vector<block>>& cache,ll indexBits,ll blockOff,ll iter,ll ways){
    // extract index and tag from the current address 
    ll ind=getIndex(addr,indexBits,blockOff),tag=getTag(addr,indexBits,blockOff);

    // do a tag match 
    bool tagMatch=false;
    for(auto y: cache[ind]){
        if(y.valid && y.tag==tag) {
            tagMatch=true;
            break;
        }
    }
    // if there isn't a tag match, we insert a new block in the cache
    if(!tagMatch){
        // creating the new block
        block tmp={true,tag,iter};

        // here, LRU has been implemented
        // to implement LRU, each cache block has a time value, which is the time at which it was last accesed 

        // find least recently accessed block's index
        ll currTime=INT_MAX,currInd=0;
        for(int i=0;i<cache[ind].size();i++){
            if(cache[ind][i].time<currTime) {
                currTime=cache[ind][i].time;
                currInd=i;
            }
        }

        // replace the new block with the block that was accesed least recently
        cache[ind][currInd]=tmp;

        // report a miss
        return false;
    }

    // in case of a tag match, we simply update that block's time field to current time
    // indicating that it has been accessed
    else{
        for(int i=0;i<cache[ind].size();i++){
            if(cache[ind][i].valid && cache[ind][i].tag==tag){
                cache[ind][i].time=iter;
                break;
            }
        }
        //report a hit
        return true;
    }
}

int main(){

    // clock_t is a fundamental arithmetic type capable of representing clock tick counts
    // it is used to measure the execution time of this program
    clock_t start, end;
    start = clock();
    ll cacheSz,blockSz,ways,addrBits=32;
    cout<<"Enter cache size (in KB), block size (in Bytes) and associativity\n";
    cin>>cacheSz>>blockSz>>ways;

    // calculating sets, index bits, tag bits and block offset
    ll blocks=cacheSz*1024/(blockSz),sets=blocks/(ways);
    ll blockOff=(ll)log2(blockSz),indexBits=(ll)log2(sets),tagBits=addrBits-indexBits-blockOff;
    cout<<setprecision(12);

    // creating a vector of all the trace files that are used
    vector <ifstream> files(5);
    files[0].open("gcc.trace");
    files[1].open("gzip.trace");
    files[2].open("mcf.trace");
    files[3].open("swim.trace");
    files[4].open("twolf.trace");

    // vector of file names, this is useful for printing the output
    vector <string> fileNames={"gcc.trace","gzip.trace","mcf.trace","swim.trace","twolf.trace"};

    // for every file, create a new cache
    // use the run function for every input value
    // and report the hits, misses and hit rate
    for(int i=0;i<5;i++){
        cout<<endl;
        ll hit=0,miss=0,iter=0;
        block t={false,0,0};
        vector<vector<block>> cache(sets);
        for(int i=0;i<sets; i++){
            for(int j=0;j<ways;j++) cache[i].push_back(t);
        }
        cout<<"Input file: "<<fileNames[i]<<endl;
        while(1){
            char c; ll a,b;
            files[i]>>c>>hex>>a>>b;
            iter++;
            if(run(a,cache,indexBits,blockOff,iter,ways)) hit++;   
            else miss++;
            if(files[i].eof()) break;
        }
        files[i].close();
        cache.clear();
        cout<<"Hit Rate = "<<100.0*(hit)/(iter)<<"%"<<endl;
        cout<<"Hits = "<<hit<<sp<<sp<<"||"<<sp<<sp<<"Misses = "<<miss<<endl;
    } 
    
    // printing the final execution time of the entire program
    end = clock();
    double time_taken = double(end - start)/double(CLOCKS_PER_SEC);
    cout<<endl<<"Program executed in "<<time_taken<<" seconds\n";
    return 0;
}
