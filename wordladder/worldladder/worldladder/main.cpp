#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <unordered_set>
#include <set>
#include <stack>
#include <queue>
using namespace std;

class Solution {
public:
    inline unsigned getDis(unsigned i, unsigned j)
    {
        unsigned index = i * m_listsize + j;
        return m_edges[index];
    }

    inline void setDis(unsigned i, unsigned j, unsigned val)
    {
        unsigned index = i * m_listsize +j;
        m_edges[index] = val;
        index = j * m_listsize + i;
        m_edges[index] = val;
    }

    inline unsigned edgeHash(unsigned i, unsigned j)
    {
        if (i < j)
        {
            return i * 33 + j;
        }
        else
        {
            return j * 33 + i;
        }
    }

    inline bool edgeProcessed(unsigned i, unsigned j, set<unsigned>& processed)
    {
        return processed.find(edgeHash(i, j)) != processed.cend();
    }

    inline bool nodeProcessed(unsigned node, set<unsigned>& processed)
    {
        for (auto i = 0; i < m_listsize; ++i)
        {
            if (getDis(node, i) > 0 && !edgeProcessed(node, i, processed))
            {
                return false;
            }
        } 

        return true;
    }

    vector<vector<string>> findLadders(string beginWord, string endWord, unordered_set<string> &wordList) 
    {
        auto listSize = wordList.size();
        m_listsize = listSize;
        m_edges.resize(listSize * listSize);
        vector<set<unsigned> > nextIds;
        nextIds.resize(listSize);
        unsigned int index = 0;
        for (auto i = wordList.cbegin(); i != wordList.cend(); ++i)
        {
            m_id2str.insert(pair<unsigned, string>(index, *i));
            m_str2id.insert(pair<string, unsigned>(*i, index));
            m_words.push_back(*i);
            m_ids.push_back(index);
            cout << index << " -> " << *i << endl;
            ++index;
        }

        for (auto i = 0; i < listSize; ++i)
        {
            for (auto j = 0; j < listSize; ++j)
            {
                if (i == j)
                {
                    continue;
                }

                if (canTransTo(m_words[i], m_words[j]))
                {
                    setDis(i, j, 1);
                    nextIds[i].insert(j);
                    nextIds[j].insert(i);
                }
            }
        }

        vector<string> v;
        vector<vector <string> > vl;
        auto startIndex = m_str2id[beginWord];
        auto endIndex = m_str2id[endWord];
        if (startIndex == endIndex)
        {
            v.push_back(beginWord);
            vl.push_back(v);
            return vl;
        }
        
        //new solution
        set<unsigned> processedEdges;
        queue<unsigned> nodes;
        nodes.push(startIndex);
        map<unsigned, unsigned> nodeLevel;
        nodeLevel[startIndex] = 0;
        while (nodes.size() != 0)
        {
            auto curnode = nodes.front();
            if (nodeProcessed(curnode, processedEdges))
            {
                nodes.pop();
                continue;
            }

            for (auto i = 0; i < m_listsize; ++i)
            {
                auto ehash = edgeHash(curnode, i);
                if (processedEdges.find(ehash) == processedEdges.cend() && getDis(curnode, i) > 0)
                {
                    nodes.push(i);
                    processedEdges.insert(ehash);
                    nodeLevel[i] = nodeLevel[curnode] + 1;
                }
            }

            if (nodeProcessed(curnode, processedEdges))
            {
                nodes.pop();
            }
        }

        //old solution
        bool changed = false;
        while (true)
        {
            changed = false;
            for (auto i = 0; i < listSize; ++i)
            {
                if (i == startIndex)
                {
                    continue;
                }

                for (auto j = 0; j < listSize; ++j)
                {
                    if (j == startIndex || i == j)
                    {
                        continue;
                    }

                    auto dissj = getDis(startIndex, j);
                    auto dissi = getDis(startIndex, i);
                    auto disij = getDis(i, j);
                    if (dissj == 0)
                    {
                        if (dissi != 0 && disij != 0)
                        {
                            setDis(startIndex, j, dissi + disij);
                            nextIds[i].insert(j);
                            changed = true;
                        }
                    }
                    else
                    {
                        if (dissi != 0 && disij != 0 && disij + dissi < dissj)
                        {
                            setDis(startIndex, j, dissi + disij);
                            nextIds[i].clear();
                            nextIds[i].insert(j);
                            changed = true;
                        }
                        else if (dissi != 0 && disij != 0 && disij + dissi == dissj && nextIds[i].find(j) == nextIds[i].cend())
                        {
                            nextIds[i].insert(j);
                            changed = true;
                        }
                    }
                }
            }

            if (!changed)
            {
                break;
            }
        }
        
        

        //cout << "."<< endl;
        map<unsigned, unsigned> processed;
        processed[startIndex] = 1;
        auto paths = getPathsTo(startIndex, endIndex, processed, nextIds);
        unsigned minLen = 0xffffffff;
        for (auto itor = paths.cbegin(); itor != paths.cend(); ++itor)
        {
            if ((*itor).size() < minLen)
            {
                minLen = (*itor).size();
            }
        }

        for(auto vecItor = paths.cbegin(); vecItor != paths.cend(); ++vecItor)
        {
            if ((*vecItor).size() == minLen)
            {
                vector<string> v;
                for (auto idItor = vecItor->cbegin(); idItor != vecItor->cend(); ++idItor)
                {
                    v.push_back(m_words[*idItor]);
                }
                vl.push_back(v);
            }
        }
        return vl;
    }
    
    vector<vector<unsigned> > getPathsTo(unsigned ufrom, unsigned uto, map<unsigned, unsigned> processed, vector<set<unsigned> >& nextIds)
    {
        vector<unsigned> v;
        vector<vector<unsigned> > vv;
        if(ufrom == uto)
        {
            v.push_back(ufrom);
            vv.push_back(v);
            return vv;
        }
        
        set<unsigned> connectTo = nextIds[ufrom];
        for(auto i = connectTo.cbegin(); i != connectTo.cend(); ++i)
        {
            if(processed.find(*i) == processed.cend())
            {
                processed[*i] = 1;
                if(uto == *i)
                {
                    v.push_back(ufrom);
                    v.push_back(uto);
                    vv.push_back(v);
                }
                else
                {
                    auto l = getPathsTo(*i, uto,  processed, nextIds);
                    if(l.size() > 0)
                    {
                        for(auto vecItor = l.cbegin(); vecItor != l.cend(); ++vecItor)
                        {
                            vector<unsigned> vec;
                            vec.push_back(ufrom);
                            vec.insert(vec.cend(), vecItor->cbegin(), vecItor->cend());
                            vv.push_back(vec);
                        }
                    }

                }
            }
            else
            {
                ++processed[*i];
            }
        }
        
        return vv;
    }

    bool canTransTo(string beginWord, string endWord)
    {
        if (beginWord.length() != endWord.length())
        {
            return false;
        }

        int diffnum = 0;
        for (auto i = 0; i < beginWord.size(); ++i)
        {
            if (beginWord[i] != endWord[i])
            {
                ++diffnum;
                if (diffnum > 1)
                {
                    return false;
                }
            }
        }

        return diffnum == 1;
    }

private:
    unsigned m_listsize;
    vector<unsigned> m_edges;
    map<unsigned, string> m_id2str;
    map<string, unsigned> m_str2id;
    vector<string> m_words;
    vector<unsigned> m_ids;
};

void Test(string from, string to, vector<string> testsample, Solution& solution)
{
    unordered_set<string> s;
    for (auto i = testsample.begin(); i != testsample.end(); ++i)
    {
        s.insert(*i);
    }

    auto paths = solution.findLadders(from, to, s);
    cout<<"Answer " << from << " --> " << to << ":" << endl;
    for(auto vecItor = paths.cbegin(); vecItor != paths.cend(); ++vecItor)
    {
        for(auto strItor = vecItor->cbegin(); strItor != vecItor->cend(); ++strItor)
        {
            cout << *strItor << " -> ";
        }
        cout<< "$" << endl;
    }
}

int main(int argc, char **argv)
{
    Solution solution;
    vector<string> s;
    //Test case 1
    
    s.push_back("hit");
    s.push_back("hot");
    s.push_back("lot");
    s.push_back("lit");
    s.push_back("lis");
    s.push_back("kis");
    Test(string("hit"), string("kis"), s, solution);
    

    //test case 2
    /*
    s.push_back("si");
    s.push_back("go");
    s.push_back("se");
    s.push_back("cm");
    s.push_back("so");
    s.push_back("ph");
    s.push_back("mt");
    s.push_back("db");
    s.push_back("mb");
    s.push_back("sb");
    s.push_back("kr");
    s.push_back("ln");
    s.push_back("tm");
    s.push_back("le");
    s.push_back("av");
    s.push_back("sm");
    s.push_back("ar");
    s.push_back("ci");
    s.push_back("ca");
    s.push_back("br");
    s.push_back("ti");
    s.push_back("ba");
    s.push_back("to");
    s.push_back("ra");
    s.push_back("fa");
    s.push_back("yo");
    s.push_back("ow");
    s.push_back("sn");
    s.push_back("ya");
    s.push_back("cr");
    s.push_back("po");
    s.push_back("fe");
    s.push_back("ho");
    s.push_back("ma");
    s.push_back("re");
    s.push_back("or");
    s.push_back("rn");
    s.push_back("au");
    s.push_back("ur");
    s.push_back("rh");
    s.push_back("sr");
    s.push_back("tc");
    s.push_back("lt");
    s.push_back("lo");
    s.push_back("as");
    s.push_back("fr");
    s.push_back("nb");
    s.push_back("yb");
    s.push_back("if");
    s.push_back("pb");
    s.push_back("ge");
    s.push_back("th");
    s.push_back("pm");
    s.push_back("rb");
    s.push_back("sh");
    s.push_back("co");
    s.push_back("ga");
    s.push_back("li");
    s.push_back("ha");
    s.push_back("hz");
    s.push_back("no");
    s.push_back("bi");
    s.push_back("di");
    s.push_back("hi");
    s.push_back("qa");
    s.push_back("pi");
    s.push_back("os");
    s.push_back("uh");
    s.push_back("wm");
    s.push_back("an");
    s.push_back("me");
    s.push_back("mo");
    s.push_back("na");
    s.push_back("la");
    s.push_back("st");
    s.push_back("er");
    s.push_back("sc");
    s.push_back("ne");
    s.push_back("mn");
    s.push_back("mi");
    s.push_back("am");
    s.push_back("ex");
    s.push_back("pt");
    s.push_back("io");
    s.push_back("be");
    s.push_back("fm");
    s.push_back("ta");
    s.push_back("tb");
    s.push_back("ni");
    s.push_back("mr");
    s.push_back("pa");
    s.push_back("he");
    s.push_back("lr");
    s.push_back("sq");
    s.push_back("ye");
    Test(string("qa"), string("sq"), s, solution);
    */
    getchar();
    return 0;
}
