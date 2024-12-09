#include <stdio.h>    
#include <stdlib.h>
#include <string.h>

typedef enum VarType {
    INPUT = 0,
    OUTPUT = 1,
    TEMP = 2
} VarType;

typedef struct VarNode VarNode;
typedef struct Gate Gate;

struct VarNode{
    char val[17];
    int value;
    struct VarNode* next; 
    Gate *parentGate;
    VarType type; // INPUT/OUTPUT/TEMP
}; 

struct Gate{
    char name[13];  
    int numInputs;
    struct VarNode** inputs;
    int numOutputs;
    struct VarNode** outputs;
    int numSelectors;
    struct VarNode** selectors; 
    struct Gate* next; 
}; 

VarNode *search(VarNode *head, char *target){
    VarNode *cur = head; 
    while(cur != NULL){
        if(strcmp(cur->val, target) == 0)
            return cur;
        cur = cur -> next; 
    }
    return NULL; 
}

void printVarList(VarNode *head){
    VarNode *cur = head;
    printf("VarHead Linked LIST: \n");
    while(cur != NULL){
        printf("value = %d ", cur->value);
        printf("name= %s ", cur->val);
        printf("TYPE= %d \n", cur->type);
        cur = cur->next; 
    }
}

void printGateList(Gate *head){
    Gate *cur = head;
    printf("Gate Linked List: \n");
    while(cur != NULL){

        printf("Name: %s \n", cur->name);
        //print numinputs and inputs array
        printf(" numInputs: %d | Array Values: ", cur -> numInputs);
            for(int i = 0; i < cur->numInputs; i ++){
                printf(" inputs[%d]: %s",i, cur->inputs[i]->val);
            }
        printf("\n");
        //print num ouputs and outputs array 
        printf(" numOutputs: %d | Array Values: ", cur -> numOutputs);
            for(int i = 0; i < cur->numOutputs; i ++){
                printf(" outputs[%d]: %s",i, cur->outputs[i]->val);
            }
        printf("\n");
        //print num selectors and selectors array 
        printf(" numSelectors: %d | Array values: ", cur -> numSelectors);
            for(int i = 0; i < cur->numSelectors; i ++){
                printf(" selectors[%d]: %s",i, cur->selectors[i]->val);
            }
        printf("\n\n");
        cur = cur->next; 
    }
}

VarNode *addTempNode(VarNode *head, char *value){
    VarNode *add = malloc(sizeof(VarNode));
    strcpy(add->val, value);
    add->parentGate = NULL;
    add->next = NULL; 
    add->type = TEMP; 

    VarNode *cur = head; 
    VarNode *prev; 
    while(cur != NULL){
        prev = cur;
        cur = cur->next; 
    }
    prev->next = add; 
    return add;
}

VarNode **readVars(VarNode *head, FILE *file, int num) {
    VarNode **vars = malloc(sizeof(VarNode *) * num);
    char buffer[17];
    for(int currentIteration = 0; currentIteration < num ;currentIteration++) {
        fscanf(file, " %16s", buffer);
        VarNode *var = search(head, buffer);
        if(var == NULL) {
            var = addTempNode(head, buffer);
        }
        vars[currentIteration] = var;
    }
    return vars;
}

Gate *makeGate(VarNode *head, FILE *file, int numSelectors, int numInputs, int numOutputs, char *op) {
    Gate *gate = malloc(sizeof(Gate));
    strcpy(gate->name, op);
    gate->next = NULL;
    gate->numInputs = numInputs;
    gate->inputs = readVars(head, file, numInputs);
    gate->numSelectors = numSelectors;
    gate->selectors = readVars(head, file, numSelectors);
    gate->numOutputs = numOutputs;
    gate->outputs = readVars(head, file, numOutputs);
    return gate;
}

Gate *addGate(Gate *head, Gate *gate) {
    if(head == NULL) {
        return gate;
    }
    Gate *cur = head;
    while(cur->next != NULL) {
        cur = cur->next;
    }
    cur->next = gate;
    return head;
}

//works
int andGate(int x, int y){
    if(x == 1 && y == 1) return 1;
    return 0; 
}

//works 
int orGate(int x, int y){
    if(x == 0 && y == 0) return 0;
    return 1; 
}

int nandGate(int x, int y){
    if( x == 1 && y == 1) return 0;
    return 1; 

   /*  if(!(x && y)){
        return 1;
    } else {
        return 0 ;
    } */
}

int norGate(int x, int y){
    if(x == 0 && y == 0) {
        return 1;
    }else {
        return 0;
    }

    /*if(!(x || y)){
        return 1;
    } else {
        return 0; 
    } */
}

//works
int xorGate(int x, int y){
    if( x== 1 && y == 0){
        return 1;
    } else if (x == 0 && y == 1){
        return 1;
    } else {
        return 0; 
    }
}

//works
int notGate(int x){
    if(x == 0) {
        return 1;
    } else {
        return 0; 
    }
}

//works
int passGate(int x){
    return x; 
}

//works
void decoderGate(Gate *head) {
    Gate *cur = head; 
    int inputValue = 0;
    // calculate input value given inputs (inputs are each one binary digit)
    for(int i = 0; i < cur->numInputs; i++){
        inputValue += cur->inputs[i]->value * (1 << (cur->numInputs - i - 1)); 
    }
    // set all outputs to 0
    for(int i = 0; i < (1 << cur->numInputs); i ++){
        cur->outputs[i]->value = 0; 
    }
    // set the gate->outputs[inputValue]->value = 1;
    cur->outputs[inputValue]->value = 1; 
}

void multGate(Gate *head){
    Gate *cur = head; 
    int selectorValue = 0;
    // calculate selectorValue given selectors (selectors are each one binary digit);
     for(int i = 0; i < cur->numSelectors; i ++){
        selectorValue += cur->selectors[i]->value * (1 << (cur-> numSelectors - i - 1)); 
    } 
    //cur->outputs[0] -> value = cur ->inputs[selectorValue]->value; 
    cur->outputs[0]->value = cur->inputs[selectorValue]->value; 
}

void printOutput(VarNode *head){
    VarNode *cur;

    cur = head; 
    while(cur != NULL) {
        if(cur->type == INPUT) {
            printf("%d ", cur->value);
        }
        cur = cur->next;
    }
    printf("|");
    cur = head;
    while(cur != NULL) {
        if(cur->type == OUTPUT) {
            printf(" %d", cur->value);
        }
        cur = cur->next;
    }
    printf("\n");
}

void resetVariables(VarNode *head){
    VarNode *cur = head;
    while(cur != NULL){
        cur->value = -1;
        cur = cur->next; 
    }
}

int runGate(Gate *head){
    Gate *cur = head; 
    for(int i = 0; i < cur->numInputs; i++) {
        if(cur->inputs[i]->value < 0) {
            return 0;
        }
    }
    for(int i = 0; i < cur->numSelectors; i++) {
        if(cur->selectors[i]->value < 0) {
            return 0;
        }
    }
    for(int i = 0; i < cur->numOutputs; i++) {
        if(cur->outputs[i]->value >= 0) {
            return 0;
        }
    }
    if(strcmp(cur->name,"AND") == 0) cur->outputs[0]->value = andGate(cur->inputs[0]->value, cur->inputs[1]->value);
    else if(strcmp(cur->name,"OR") == 0) cur->outputs[0]->value = orGate(cur->inputs[0]->value, cur->inputs[1]->value);
    else if(strcmp(cur->name,"NAND") == 0) cur->outputs[0]->value = nandGate(cur->inputs[0]->value, cur->inputs[1]->value);
    else if(strcmp(cur->name,"NOR") == 0) cur->outputs[0]->value = norGate(cur->inputs[0]->value, cur->inputs[1]->value);
    else if(strcmp(cur->name,"XOR") == 0) cur->outputs[0]->value =xorGate(cur->inputs[0]->value, cur->inputs[1]->value);
    else if(strcmp(cur->name,"PASS") == 0) cur->outputs[0]->value = passGate(cur->inputs[0]->value); 
    else if(strcmp(cur->name,"NOT")==0) cur->outputs[0]->value = notGate(cur->inputs[0]->value); 
    else if(strcmp(cur->name,"DECODER") == 0) decoderGate(head); 
    else if(strcmp(cur->name,"MULTIPLEXER")==0) multGate(head); 

    return 1;
}
 
void populateVariables(VarNode *varHead, int numInputs, int currentIteration) {
    VarNode *cur = varHead;
    while(cur != NULL) {
        if(cur->type == INPUT) {
            numInputs--;
            cur->value = (currentIteration >> numInputs) % 2;
        } else if(!strcmp(cur->val, "0")) {
            cur->value = 0;
        } else if(!strcmp(cur->val, "1")) {
            cur->value = 1;
        }
        cur = cur->next;
    }
}

Gate *findGateWithOutput(Gate *head, char *varName) {
    Gate *cur = head;
    while(cur != NULL) {
        for(int i = 0; i < cur->numOutputs; i++) {
            if(!strcmp(cur->outputs[i]->val, varName)) {
                return cur;
            }
        }
        cur = cur->next;
    }
    return NULL;
}

void calculateVariable(VarNode *var) {
    if(var->value >= 0) return;
    /* runGateBetter(var->parentGate); */
    Gate *gate = var->parentGate;
    for(int i = 0; i < gate->numInputs; i++) {
        calculateVariable(gate->inputs[i]);
    }
    for(int i = 0; i < gate->numSelectors; i++) {
        calculateVariable(gate->selectors[i]);
    }
    if(var->value >= 0) return;

    char *name = gate->name;
    int input1 = gate->inputs[0]->value;
    int input2 = gate->numInputs > 1 ? gate->inputs[1]->value : -1;
    int *output1 = &gate->outputs[0]->value;
    if(strcmp(name,"AND") == 0) *output1 = input1 && input2;
    else if(strcmp(name,"OR") == 0) *output1 = input1 || input2;
    else if(strcmp(name,"NAND") == 0) *output1 = !(input1 && input2);
    else if(strcmp(name,"NOR") == 0) *output1 = !(input1 || input2);
    else if(strcmp(name,"XOR") == 0) *output1 = (input1 && !input2) || (!input1 && input2);
    else if(strcmp(name,"PASS") == 0) *output1 = input1;
    else if(strcmp(name,"NOT")==0) *output1 = !input1;
    else if(strcmp(name,"DECODER") == 0) decoderGate(gate);
    else if(strcmp(name,"MULTIPLEXER")==0) multGate(gate);
}

void runCircuit(Gate* gateHead, VarNode *varHead ){
    VarNode *cur = varHead;
    while(cur != NULL) {
        if(cur->type == OUTPUT) {
            calculateVariable(cur);
        }
        cur = cur->next;
    }
}

int main(int argc, char** argv){    
    VarNode * varHead = NULL; //at
    Gate *gateHead = NULL; 
    
    //opening input file 
    FILE* fp = fopen(argv[1], "r");
    if (fp == NULL ){
        return 0; 
    }
    
    //parsing input data for linked list 
    int size1; 
    fscanf(fp, "%*s %d", &size1); 
    int counter = 0;
    //populate empty linked list with first variable
    if(varHead == NULL){
        VarNode *insert = malloc(sizeof(VarNode)); 
        fscanf(fp, "%16s", insert->val);
        insert->type = INPUT; 
        insert->next = NULL; 
        varHead = insert; 
        counter ++;
    }

    //make head temp 
    VarNode *temp1 = varHead;
    //fill in rest of LL through temp 
    while(counter < size1){
        VarNode * insert = malloc(sizeof(VarNode)); 
        fscanf(fp, "%16s", insert->val);
        insert->type = INPUT; 
        insert->next = NULL; 
        temp1->next = insert;
        temp1 = temp1->next; 
        counter ++; 
    }

    //parsing output data to make a linked list
    counter = 0;
    int size2; 
    fscanf(fp, " %*s %d", &size2); 
    while(counter < size2){
        VarNode * insert = malloc(sizeof(VarNode)); 
        fscanf(fp, "%16s", insert->val);
        insert->type = OUTPUT; 
        insert->next = NULL; 
        temp1->next = insert;
        temp1 = temp1->next; 
        counter ++; 
    }

    //populating Gate linked list
    char buffer[17];
    int numGates = 0;
    while(fscanf(fp, " %16s", buffer) > 0){
        int numSelectors = 0, numInputs, numOutputs = 1;
        if (strcmp(buffer, "AND") == 0 || strcmp(buffer, "OR") ==0 || strcmp(buffer, "NAND") == 0 || strcmp(buffer,"NOR") ==0 || strcmp(buffer,"XOR")==0){
            numInputs = 2;
        } else if(!strcmp(buffer, "PASS") || !strcmp(buffer, "NOT")) {
            numInputs = 1;
        } else if(!strcmp(buffer, "MULTIPLEXER")) {
            fscanf(fp, "%d", &numSelectors);
            numInputs = 1 << numSelectors;
        } else if(!strcmp(buffer, "DECODER")) {
            fscanf(fp, "%d", &numInputs);
            numOutputs = 1 << numInputs;
        }
        numGates++;
        Gate *gate = makeGate(varHead, fp, numSelectors, numInputs, numOutputs, buffer);
        gateHead = addGate(gateHead, gate);
    }

    VarNode *curVar = varHead;
    while(curVar != NULL) {
        Gate *gate;
        if((gate = findGateWithOutput(gateHead, curVar->val)) != NULL) {
            curVar->parentGate = gate;
        }
        curVar = curVar->next;
    }


    for(int currentIteration = 0; currentIteration < (1 << size1); currentIteration++){
        populateVariables(varHead, size1, currentIteration); //setting input variables
       // printGateList(gateHead); 
        runCircuit(gateHead, varHead);
        printOutput(varHead); // printing inputs/outputs
        resetVariables(varHead);  // resetting everything
   }

   //freeing everything 
    while(gateHead != NULL){
        Gate *temp = gateHead; 
        free(gateHead->inputs);
        free(gateHead->outputs);
        free(gateHead->selectors);
        gateHead=gateHead->next;
        free(temp); 
    }
    while(varHead != NULL){
        VarNode * temp = varHead; // 1->2->3
        varHead = varHead ->next;
        free(temp);
    }
}  



