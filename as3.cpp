/* REFERENCES 
// addu $v0, $v0, $v2
// 0x00 02 02 03
// $v0 = $v0 + $v1
// machineState[regMap[u.rFormat.rd]] = machineState[regMap[2]] + machineState[regMap[3]]
*/

#include <iostream> 
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include <map>
#include <string>
#include <vector>

typedef std::string String;
typedef std::map<String, unsigned> Map; 
typedef std::map<unsigned, String> ReverseMap;
typedef std::map<unsigned, unsigned> StateMap;
typedef std::vector<unsigned> Vector;
typedef std::vector<String> StrVector;  

union{
  unsigned encodedValue;  
  struct{
    unsigned fn :  6; 
    unsigned sa :  5; 
    unsigned rd :  5;
    unsigned rt :  5;
    unsigned rs :  5;
    unsigned op :  6;
  }rtype;  
  struct{
    unsigned im : 16; 
    unsigned rt :  5; 
    unsigned rs :  5; 
    unsigned op :  6; 
  }itype; 
  struct{
    unsigned ad : 26;  
    unsigned op :  6; 
  }jtype; 
}u;

Map regName =
{
 {"$zero",0}, {"$at",1}, {"$v0",2}, {"$v1",3}, {"$a0",4}, {"$a1",5}, {"$a2",6},
 {"$a3",7}, {"$t0",8}, {"$t1",9}, {"$t2",10}, {"$t3",11}, {"t4",12}, {"$t5",13},
 {"$t6",14}, {"$t7",15}, {"$s0",16}, {"$s1",17}, {"$s2",18}, {"$s3",19}, 
 {"$s4",20}, {"$s5",21}, {"$s6",22}, {"$s7",23}, {"$t8",24}, {"$t9",25}, 
 {"$k0",26}, {"$k1",27}, {"$gp",28}, {"$sp",29}, {"$fp",30}, {"$ra",31}, 
 {"$lo", 32}, {"$hi", 33}
 };
 
 Map machineState = 
{
  {"$zero",0}, {"$at",0}, {"$v0",0}, {"$v1",0}, {"$a0",0}, {"$a1",0}, {"$a2",0},
  {"$a3",0}, {"$t0",0}, {"$t1",0}, {"$t2",0}, {"$t3",0}, {"$t4",0}, {"$t5",0}, 
  {"$t6",0}, {"$t7",0}, {"$s0",0}, {"$s1",0}, {"$s2",0}, {"$s3",0}, {"$s4",0}, 
  {"$s5",0}, {"$s6",0}, {"$s7",0}, {"$t8",0}, {"$t9",0}, {"$k0",0}, {"$k1",0},
  {"$gp",0}, {"$sp",0}, {"$fp",0}, {"$ra",0}, {"$lo", 0}, {"$hi", 0}
}; 

ReverseMap regNum =
{
  {0,"$zero"}, {1,"$at"}, {2,"$v0"}, {3,"$v1"}, {4,"$a0"}, {5,"$a1"}, {6,"$a2"},
  {7,"$a3"}, {8,"$t0"}, {9,"$t1"}, {10,"$t2"}, {11,"$t3"}, {12,"$t4"}, 
  {13,"$t5"}, {14,"$t6"}, {15,"$t7"}, {16,"$s0"}, {17,"$s1"}, {18,"$s2"}, 
  {19,"$s3"}, {20,"$s4"}, {21,"$s5"}, {22,"$s6"}, {23,"$s7"}, {24,"$t8"}, 
  {25,"$t9"}, {26,"$k0"}, {27,"$k1"}, {28,"$gp"}, {29,"$sp"}, {30,"$fp"}, 
  {31,"$ra"}, {32, "$lo"}, {33, "$hi"}
};

StateMap state = 
{  
  {0,0}, {1,0}, {2,0}, {3,0}, {4,0}, {5,0}, {6,0}, {7,0}, {8,0}, {9,0}, {10,0},
  {11,0}, {12,0}, {13,0}, {14,0}, {15,0}, {16,0}, {17,0}, {18,0}, {19,0},
  {20,0}, {21,0}, {22,0}, {23,0}, {24,0}, {25,0}, {26,0}, {27,0}, {28,0}, 
  {29,0}, {30,0}, {31,0}, {32, 0}, {33, 0}
};

// debugging 
void dumpLine(const Vector &v){
  std::cout << "\n*** LINE VECTOR DUMP ***\n"; 
  for(int i = 0; i < v.size(); ++i)
    std::cout << "Line [" << i << "] == " << v[i] << std::endl; 
}

/* DECODING FUNCTIONS */ 
//TODO --  j-type function 
// WRITE TO FILE 
void decode(const Vector &v, unsigned num){
  
  std::cout << "insts:\n"; 
  int k = 0; 
  while(k < v.size() - num)
  { 
    u.encodedValue = v[k]; 
    
    if(u.rtype.op == 0)
    {
      if(u.rtype.fn == 33){
        std::cout << std::setw(4) 
        << k << ": " << "addu  " << regNum[u.rtype.rd] 
        << "," << regNum[u.rtype.rs] << "," << regNum[u.rtype.rt] << std::endl; 
      }
      else if(u.rtype.fn == 36){
        std::cout << std::setw(4) << k << ": " << "and " << regNum[u.rtype.rd] << "," 
        << regNum[u.rtype.rs] << "," << regNum[u.rtype.rt] << std::endl;
      }
      else if(u.rtype.fn == 26){
        std::cout << std::setw(4)<< k << ": " << "div " << regNum[u.rtype.rs] << ","
        << regNum[u.rtype.rt] << std::endl; 
      }
      else if(u.rtype.fn == 37){
        std::cout << std::setw(4)<< k << ": "  << "or " << regNum[u.rtype.rd] << "," 
        << regNum[u.rtype.rs] << "," << regNum[u.rtype.rt] << std::endl;
      }
      else if(u.rtype.fn == 35){
        std::cout << std::setw(4)<< k << ": " << "subu " << regNum[u.rtype.rd] << "," 
        << regNum[u.rtype.rs] << "," << regNum[u.rtype.rt] << std::endl;
      }
      else if(u.rtype.fn == 42){
        std::cout << std::setw(4) << k << ": " << "slt " << regNum[u.rtype.rd] << "," 
        << regNum[u.rtype.rs] << "," << regNum[u.rtype.rt] << std::endl;
      }
      else if(u.rtype.fn == 24){
        std::cout << std::setw(4)<< k << ": " << "mult " << regNum[u.rtype.rs] << "," 
        << regNum[u.rtype.rt] << std::endl;
      }
      else if(u.rtype.fn == 16){
        std::cout << std::setw(4)<< k << ": " << "mfhi " << regNum[u.rtype.rs] 
        << std::endl;
      }
      else if(u.rtype.fn == 18){
        std::cout << std::setw(4)<< k << ": " << "mflo " << regNum[u.rtype.rs] 
        << std::endl; 
      }
      else if(u.rtype.fn == 12){
        std::cout << std::setw(4)<< k << ": " << "syscall\n";
      }
    }
    else if(u.jtype.op == 2){
      std::cout << std::setw(4)<< k << ": " << "j" << std::endl;  
    }
    else
    {
      if(u.itype.op == 9){
        std::cout << std::setw(4)<< k << ": "  << "addiu " << regNum[u.itype.rt] 
        << "," << regNum[u.itype.rs] << "," << u.itype.im << std::endl; 
      }
      else if(u.itype.op == 4){
        std::cout << std::setw(4)<< k << ": "  << "beq " << regNum[u.itype.rs] << "," 
        << regNum[u.itype.rt] << "," << u.itype.im <<std::endl; 
      }
      else if(u.itype.op == 5){
        std::cout << std::setw(4)<< k << ": " << "bne " << regNum[u.itype.rs] << "," 
        << regNum[u.itype.rt] << u.itype.im << std::endl; 
      }
      else if(u.itype.op == 35){
        std::cout << std::setw(4)<< k << ": " << "lw  " << regNum[u.itype.rt] << "," 
        << u.itype.im << "(" << regNum[u.itype.rs] << ")" << std::endl; 
      }
      else if(u.itype.op == 43){
        std::cout << std::setw(4)<< k << ": " << "sw  " << regNum[u.itype.rt] << "," 
        << u.itype.im  << "(" << regNum[u.itype.rs] << ")" << std::endl; 
      } 
    }   
    ++k; 
  }

  // print data -- format width 
  std::cout << "\ndata:\n"; 
  for(int j = k; j < v.size(); ++j){
    std::cout << std::setw(4)<< k++ << ": " << v[j] << std::endl; 
  }  
}

/* SIMULATION FUNCTIONS */ 


void rsim(Vector &v){
  /*
  if(u.rFormat.rd == 0){
    std::cout << "Illegal assignment to $zero\n";
    return; 
  }*/
  
  // ADDU: RD = RS + RT 
  if(u.rtype.fn == 33){
    
    // print instruction 
    std::cout << "inst: addu " << regNum[u.rtype.rd] << "," 
    << regNum[u.rtype.rs] << "," << regNum[u.rtype.rt] << std::endl; 
  }
  
  // AND -- RD = RS & RT 
  else if(u.rtype.fn == 36){
    
     // print instruction 
     std::cout << "inst: and " << regNum[u.rtype.rd] << "," 
     << regNum[u.rtype.rs] << "," << regNum[u.rtype.rt] << std::endl;
  }
  
  // DIV -- RS / RT; Quotient in LO, Remainder in HI; DIV 0 ERROR 
  else if(u.rtype.fn == 26){
    
    // print instruction 
    std::cout << "inst: div " << regNum[u.rtype.rs] << ","
    << regNum[u.rtype.rt] << std::endl; 
  }
  
  // OR -- RD = RS ^ RT 
  else if(u.rtype.fn == 37){
    
    // print instruction 
    std::cout << "inst: or " << regNum[u.rtype.rd] << "," 
    << regNum[u.rtype.rs] << "," << regNum[u.rtype.rt] << std::endl;
  }
  
  // SUBU -- RD = RS - RT 
  else if(u.rtype.fn == 35){
    
    // print instruction 
    std::cout << "inst: subu " << regNum[u.rtype.rd] << "," 
    << regNum[u.rtype.rs] << "," << regNum[u.rtype.rt] << std::endl;
  }
  
  // SLT -- RD = RS < RT -> 1 ELSE RD == 0 
  else if(u.rtype.fn == 42){
    
    // print instruction 
    std::cout << "inst: slt " << regNum[u.rtype.rd] << "," 
    << regNum[u.rtype.rs] << "," << regNum[u.rtype.rt] << std::endl;
  }
  
  // MULT: RS * RT; low-order word in LO, high-order word in HI 
  else if(u.rtype.fn == 24){
    
    // print instruction 
    std::cout << "inst: mult " << regNum[u.rtype.rs] << "," 
    << regNum[u.rtype.rt] << std::endl;
  }
  
  // MFHI -- RD; Move HI to RD 
  else if(u.rtype.fn == 16){
    
    // print instruction 
    std::cout << "inst: mfhi " << regNum[u.rtype.rs] << std::endl;
  }
      
  // MFLO -- RD; Move LO to RD 
  else if(u.rtype.fn == 18){
    
    // print instruction 
    std::cout << "inst: mflo " << regNum[u.rtype.rs] << std::endl; 
  }
    
  // SYSCALL   
  else if(u.rtype.fn == 12){
    
    // print instruction 
    std::cout << "inst: syscall\n";

  }
  else
    std::cout << "rsim() error...\n";
  
  return; 
}

int main(int argc, char* argv[])
{
  bool advance = false; 
  int pc = 0; 
  int pcMin = 0; 
  int pcMax = -1; 
  int dataStart = -1; 
  unsigned ncode = 0; 
  unsigned textlines = 0; 
  unsigned numWords = 0; 
  unsigned lineNumber = 0; 
  String line; 
  Vector lines; 
  Vector dataWords; 
   
  // check for command line argument error
  if(argc != 2){ 
    std::cout << "usage: sim.exe <name>.obj\n"; 
    return EXIT_FAILURE;
  }  
  
  // create file streams -- inputFile and outputFile
  std::ifstream inputFile(argv[1]);
  
  
  // read file 
  while(std::getline(inputFile, line)){
    
    // extract header information 
    if(sscanf(line.c_str(), "%d %d", &textlines, &numWords) == 2)
    {
      dataStart = textlines; 
      pcMax = textlines + numWords - 1; 
    }
    // extract hex lines 
    else if(sscanf(line.c_str(), "%x", &ncode) == 1)
    {
      lines.push_back(ncode); 
      ++lineNumber; 
    }
    // error handling 
    else 
    {
      std::cout << "PARSING ERROR: Unknown machine code format on line "
      << lineNumber << std::endl; 
    }
    
    line.clear(); 
  }

  // store dataStart line into $gp
  machineState["$gp"] = dataStart;
  
  /* PRINT OUT ASSEMBLY FILE */ 
  decode(lines, numWords); 
  
  // spacer 
  std::cout << std::endl; 
  
  /* EXECUTION LOOP */ 
  while(pc < dataStart)
  {
    advance = true; 
    
    u.encodedValue = lines[pc]; 
    std::cout << "PC: " << pc << std::endl; 
    
    // R-FORMAT INSTRUCTIONS
    if(u.rtype.op == 0){
      rsim(dataWords); 
    }
    
    // J-FORMAT INSTRUCTIONS
    else if(u.jtype.op == 2){
      std::cout << "jsim() called...\n"; 
      //jsim(); 
    }
    
    // I-FORMAT INSTRUCTIONS
    else
    {
      std::cout << "isim() called...\n"; 
     //isim(advance, pc, pcMin, pcMax, dataWords);
    }
    
    if(advance)
      ++pc; 
    
    // spacer 
    std::cout << std::endl; 
    std::cout << std::endl; 
  }
     
  //close file 
  inputFile.close(); 
  
  return 0; 
}





