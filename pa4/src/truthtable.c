#include<stdlib.h>
#include<stdio.h>
#include<string.h>

int strequals(char *s1, char *s2);
int **makeidentitymatrix(int n);

typedef enum { AND, OR, NAND, NOR, XOR, NOT, PASS, DECODER, MULTIPLEXER } kind_t;

struct Variable {
    int unique_id;
    char *name;
    int value;
    struct Variable *next;
};
typedef struct Variable Variable;

struct Gate {
    kind_t kind;
    int size;
    int *params;
    struct Gate *next;
};
typedef struct Gate Gate;

struct Circuit {
    Variable *inputLL;
    Variable *outputLL;
    Variable *tempLL;
    Gate *gateLL; 
};
typedef struct Circuit Circuit;

Variable generatevar(char *name, int *next_unique) {
    Variable temp;
    temp.unique_id = *next_unique;
    *next_unique = *next_unique + 1;
    temp.name = name;
    temp.next = NULL;
    return temp;
}

void and(Variable *input1, Variable *input2, Variable *output) {
    int x, y;
    x = input1->value;
    y = input2->value;

    output->value = x * y; 
}

void or(Variable *input1, Variable *input2, Variable *output) {
    int x, y;
    x = input1->value;
    y = input2->value;

    if(x == 1 && y == 1) {
        output->value = 1;
    } else {
        output->value = x + y; 
    }
}

void nand(Variable *input1, Variable *input2, Variable *output) {
    int x, y;
    x = input1->value;
    y = input2->value;

    if(x==1 && y==1) {
        output->value = 0;
    } else {
        output->value = 1;
    }

}

void nor(Variable *input1, Variable *input2, Variable *output) {
    int x, y;
    x = input1->value;
    y = input2->value;

    if(x==0 && y==0) {
        output->value = 1;
    } else {
        output->value = 0;
    }
}

void xor(Variable *input1, Variable *input2, Variable *output) {
    int x, y;
    x = input1->value;
    y = input2->value;

    if(x==1 && y==1) {
        output->value = 0;
    } else {
        output->value = x + y;
    }
}

void pass(Variable *input, Variable *output) {
    output->value = input->value;
}

void not(Variable *input, Variable *output) {
    output->value = !(input->value);
}

Variable *getvarfromuniqueid(int unique_id, Circuit *c) {
    //Search the input LL
    Variable *temp = c->inputLL;
    while(temp != NULL){
        if(temp->unique_id == unique_id) {
            return temp;
        } else {
            temp = temp->next;
        }
    }

    temp = c->tempLL;
    while(temp != NULL){
        if(temp->unique_id == unique_id) {
            return temp;
        } else {
            temp = temp->next;
        }
    }

    temp = c->outputLL;
    while(temp != NULL){
        if(temp->unique_id == unique_id) {
            return temp;
        } else {
            temp = temp->next;
        }
    }
    return NULL;
}

int getuniqueidfromvar(char *name, Circuit *c) {
    //Search the input LL
    Variable *temp = c->inputLL;
    while(temp != NULL){
        if(strequals(temp->name, name)) {
            return temp->unique_id;
        } else {
            temp = temp->next;
        }
    }

    temp = c->tempLL;
    while(temp != NULL){
        if(strequals(temp->name, name)) {
            return temp->unique_id;
        } else {
            temp = temp->next;
        }
    }

    temp = c->outputLL;
    while(temp != NULL){
        if(strequals(temp->name, name)) {
            return temp->unique_id;
        } else {
            temp = temp->next;
        }
    }

    return -1;
}

void printmatrix(int **matrix, int n, int m) {
    int i;
    int j;


    for(i = 0; i < n; i++) {
        for(j=0; j < m; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int **allocatematrix(int row, int col) {
    int **mat = malloc(sizeof(double*)*row);
    int i;
    for (i = 0; i<row; i++) {
        mat[i] = malloc(sizeof(int)*col);
    }
    return mat;
}

int **makeidentitymatrix(int n) {
    int **idmat = allocatematrix(n,n);

    int i; 
    int j;


    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if(j == i) {
                idmat[i][j] = 1;
            } else {
                idmat[i][j] = 0;
            }
        }
    }
    return idmat;
}

void freematrix(int **mat, int row)
{
    int i;

    for (i = 0; i < row; i++) {
        free(mat[i]);
    }
    free(mat);
}

int **generatetruthtableinputs(int numinputs) {
    int numcols = numinputs;
    int numrows = 1 << numcols;

    int **matrix = allocatematrix(numrows, numcols);
    int row, col;

    int colnum = 0;
    int alternator = 1 << colnum;
    int count = 0;
    int value = 0;

    for(col = numcols - 1; col >= 0; col--){
        for(row = 0; row < numrows; row++) {
            matrix[row][col] = value;
            count++;
            if(count == alternator){
                value = !value;
                count = 0;
            }
        }
        colnum++;
        alternator = 1 << colnum;
    }

    return matrix;

}

int getnuminputs(Circuit *c) {
    int count = 0;
    Variable *current = c->inputLL;
    while(current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

void appendvartocircuitlist(Variable **head, Variable **var) {
    if(*head == NULL) {
        *head = *var;
        return;
    }

    Variable *last = *head;

    while(last->next != NULL) {
        last = last->next;
    }
    last->next = *var;
}

void appendgatetocircuit(Gate **head, Gate **g) {
    if(*head == NULL) {
        *head = *g;
        return;
    } 

    Gate *last = *head;

    while(last->next != NULL) {
        last = last->next;
    }
    last->next = *g;
}

Variable *allocatevariable(){
    Variable *temp = (Variable*)malloc(sizeof(Variable));
    temp->name = (char*)malloc(17*sizeof(char));
    return temp;
}

int strequals(char *s1, char *s2) {
    int i = 0;
    while(s1[i] != '\0') {
        if(s1[i] != s2[i]) {
            return 0;
        }
        i++;
    }

    if(s2[i] != '\0') {
        return 0;
    }
    return 1;
}

void freevariable(Variable *var) {
    free(var->name);
    free(var);
}

void freegate(Gate *gate) {
    free(gate->params);
    free(gate);
}

int matcharray(int len, int *arr1, int *arr2) {
    int i = 0;
    for(i = 0; i < len; i++) {
        if(arr1[i] != arr2[i]){
            return 0;
        }
    }
    return 1;
}

int getdecimalfromcoef(int size, int *coef) {
    int dec = 0;
    int exp = size - 1;
    for(int i = 0; i < size; i++) {
        if(coef[i] == 1) {
            dec = dec + (1 << exp);
        }
        exp--;
    }
    return dec;
}

void multiplexer(int numselectors, int *params, Circuit *c) {
    int numinputs = 1 << numselectors;
    int paramsize = numinputs + numselectors + 1;

    int *inputs = (int*)malloc(numinputs*sizeof(int));
    int *selectors = (int*)malloc(numselectors*sizeof(int));
    int output = params[paramsize - 1];


    int i;
    for(i = 0; i < numinputs; i++) {
        inputs[i] = params[i];
    }
    
    for(i = 0; i < numselectors; i++) {
        selectors[i] = params[numinputs + i]; 
    }

    // Fill inputs array and selectors w values
    Variable *tempvar;
    for(i = 0; i < numinputs; i++) {
        tempvar = getvarfromuniqueid(inputs[i], c);
        inputs[i] = tempvar->value;
    }

    for(i = 0; i < numselectors; i++) {
        tempvar = getvarfromuniqueid(selectors[i], c);
        selectors[i] = tempvar->value;
    }

    int selectordecimal = getdecimalfromcoef(numselectors, selectors);

    Variable *outputvar = getvarfromuniqueid(output, c);
    outputvar->value = inputs[selectordecimal];


    free(inputs);
    free(selectors);
}


void decoder(int numinputs, int *params, Circuit *c) {
    int rows = 1 << numinputs;
    // int cols =  numinputs;
    int **inputsmatrix = generatetruthtableinputs(numinputs);
    int **outputsmatrix = makeidentitymatrix(rows);

    int* inputs = (int*)malloc(numinputs*sizeof(int));
    int* outputs = (int*)malloc(rows*sizeof(int));

    // Fill inputs/outputs arr
    int i;
    for(i = 0; i < numinputs; i++) {
        inputs[i] = params[i];
    }
    
    for(i = 0; i < rows; i++) {
        outputs[i] = params[numinputs + i]; 
    }

    // Fill inputs array w values
    Variable *tempvar;
    for(i = 0; i < numinputs; i++) {
        tempvar = getvarfromuniqueid(inputs[i], c);
        inputs[i] = tempvar->value;
    }


    i = 0;
    while(!matcharray(numinputs, inputs, inputsmatrix[i])) {
        i++;
    }

    // i should be set to the row we matched inputs to

    //write inputs to output params
    int j;
    for(j = 0; j < rows; j++) {
        tempvar = getvarfromuniqueid(outputs[j], c);
        
        if(!strequals(tempvar->name, "_")) {
            tempvar->value = outputsmatrix[i][j];
        }
    }


    free(inputs);
    free(outputs);
    freematrix(inputsmatrix, rows);
    freematrix(outputsmatrix, rows);
}

int main(int argc, char **argv) {
    Circuit c = {NULL, NULL, NULL, NULL};
    int next_unique_id = 0;

    FILE *fp;
    fp = fopen(argv[1], "r");
    // fp = fopen("../data/test.1.02.txt", "r");
    char inputtag[17];
    fscanf(fp, " %16s", inputtag);
    int numinputs = 0;
    fscanf(fp, " %d", &numinputs);

    //Input generation
    int i;
    for(i = 0; i < numinputs; i++) {
        Variable *temp = allocatevariable();
        fscanf(fp, " %16s", temp->name);
        temp->unique_id = next_unique_id;
        next_unique_id++;
        temp->next = NULL;
        appendvartocircuitlist(&(c.inputLL), &temp);
    }


    char outputtag[17];
    fscanf(fp, " %16s", outputtag);
    int numoutputs = 0;
    fscanf(fp, " %d", &numoutputs);

    // Output Factory
    for(i = 0; i < numoutputs; i++) {
        Variable *temp = allocatevariable();
        fscanf(fp, " %16s", temp->name);
        temp->unique_id = next_unique_id;
        next_unique_id++;
        temp->next = NULL;
        appendvartocircuitlist(&(c.outputLL), &temp);
    }



    //Read in gates

    char gatetoken[17];
    char currentvar[17];
    while(fscanf(fp, " %16s", gatetoken) != EOF) {
        // Allocate a Gate
        Gate *gtemp = (Gate*)malloc(sizeof(Gate));
        gtemp->next = NULL;
        if(strequals(gatetoken, "AND")) {
            //Allocate params
            gtemp->params = malloc(3*sizeof(int));
            gtemp->kind = AND;
            int a;
            int id;
            for(a = 0; a < 3; a++) {  //TODO: Handle temporary inputs and 0/1s'
                fscanf(fp, " %16s", currentvar);
                id = getuniqueidfromvar(currentvar, &c);
                if (id == -1) {
                    // temp var
                    // 0 or 1
                    // don't care _
                    Variable *tempvar = allocatevariable();
                    tempvar->name = strcpy(tempvar->name, currentvar);
                    tempvar->unique_id = next_unique_id;
                    gtemp->params[a] = tempvar->unique_id;
                    next_unique_id++;
                    tempvar->next = NULL;
                    if (strequals(currentvar, "0")) {
                        tempvar->value = 0;
                    } else if (strequals(currentvar, "1")) {
                        tempvar->value = 1;
                    }
                    appendvartocircuitlist(&(c.tempLL), &tempvar);
                } else {
                    gtemp->params[a] = id;
                }
            }
            appendgatetocircuit(&(c.gateLL), &gtemp);
        } else if (gatetoken[0] == 'O') {
            gtemp->params = malloc(3*sizeof(int));
            gtemp->kind = OR;
            int a, id;
            for(a = 0; a < 3; a++){
                fscanf(fp, " %16s", currentvar);
                id = getuniqueidfromvar(currentvar, &c);
                if (id == -1) {
                    Variable *tempvar = allocatevariable();
                    tempvar->name = strcpy(tempvar->name, currentvar);
                    tempvar->unique_id = next_unique_id;
                    gtemp->params[a] = tempvar->unique_id;
                    next_unique_id++;
                    tempvar->next = NULL;
                    if (strequals(currentvar, "0")) {
                        tempvar->value = 0;
                    } else if (strequals(currentvar, "1")) {
                        tempvar->value = 1;
                    }
                    appendvartocircuitlist(&(c.tempLL), &tempvar);
                } else {
                    gtemp->params[a] = id;
                }
            }
            appendgatetocircuit(&(c.gateLL), &gtemp);
        } else if (gatetoken[0] == 'N' && gatetoken[1] == 'A' && gatetoken[2] == 'N') {
            gtemp->params = malloc(3*sizeof(int));
            gtemp->kind = NAND;
            int a, id;
            for(a = 0; a < 3; a++){
                fscanf(fp, " %16s", currentvar);
                id = getuniqueidfromvar(currentvar, &c);
                if (id == -1) {
                    Variable *tempvar = allocatevariable();
                    tempvar->name = strcpy(tempvar->name, currentvar);
                    tempvar->unique_id = next_unique_id;
                    gtemp->params[a] = tempvar->unique_id;
                    next_unique_id++;
                    tempvar->next = NULL;
                    if (strequals(currentvar, "0")) {
                        tempvar->value = 0;
                    } else if (strequals(currentvar, "1")) {
                        tempvar->value = 1;
                    }
                    appendvartocircuitlist(&(c.tempLL), &tempvar);
                } else {
                    gtemp->params[a] = id;
                }
            }
            appendgatetocircuit(&(c.gateLL), &gtemp);
        } else if (gatetoken[0] == 'N' && gatetoken[1] == 'O' && gatetoken[2] == 'R') {
            gtemp->params = malloc(3*sizeof(int));
            gtemp->kind = NOR;
            int a, id;
            for(a = 0; a < 3; a++){
                fscanf(fp, " %16s", currentvar);
                id = getuniqueidfromvar(currentvar, &c);
                if (id == -1) {
                    Variable *tempvar = allocatevariable();
                    tempvar->name = strcpy(tempvar->name, currentvar);
                    tempvar->unique_id = next_unique_id;
                    gtemp->params[a] = tempvar->unique_id;
                    next_unique_id++;
                    tempvar->next = NULL;
                    if (strequals(currentvar, "0")) {
                        tempvar->value = 0;
                    } else if (strequals(currentvar, "1")) {
                        tempvar->value = 1;
                    }
                    appendvartocircuitlist(&(c.tempLL), &tempvar);
                } else {
                    gtemp->params[a] = id;
                }
            }
            appendgatetocircuit(&(c.gateLL), &gtemp);
        } else if (gatetoken[0] == 'X'){
            gtemp->params = malloc(3*sizeof(int));
            gtemp->kind = XOR;
            int a, id;
            for(a = 0; a < 3; a++){
                fscanf(fp, " %16s", currentvar);
                id = getuniqueidfromvar(currentvar, &c);
                if (id == -1) {
                    Variable *tempvar = allocatevariable();
                    tempvar->name = strcpy(tempvar->name, currentvar);
                    tempvar->unique_id = next_unique_id;
                    gtemp->params[a] = tempvar->unique_id;
                    next_unique_id++;
                    tempvar->next = NULL;
                    if (strequals(currentvar, "0")) {
                        tempvar->value = 0;
                    } else if (strequals(currentvar, "1")) {
                        tempvar->value = 1;
                    }
                    appendvartocircuitlist(&(c.tempLL), &tempvar);
                } else {
                    gtemp->params[a] = id;
                }
            }
            appendgatetocircuit(&(c.gateLL), &gtemp);
        } else if (gatetoken[0] == 'N' && gatetoken[1] == 'O' && gatetoken[2] == 'T') {
            gtemp->params = malloc(2*sizeof(int));
            gtemp->kind = NOT;
            int a, id;
            for(a = 0; a < 2; a++){
                fscanf(fp, " %16s", currentvar);
                id = getuniqueidfromvar(currentvar, &c);
                if (id == -1) {
                    Variable *tempvar = allocatevariable();
                    tempvar->name = strcpy(tempvar->name, currentvar);
                    tempvar->unique_id = next_unique_id;
                    gtemp->params[a] = tempvar->unique_id;
                    next_unique_id++;
                    tempvar->next = NULL;
                    if (strequals(currentvar, "0")) {
                        tempvar->value = 0;
                    } else if (strequals(currentvar, "1")) {
                        tempvar->value = 1;
                    }
                    appendvartocircuitlist(&(c.tempLL), &tempvar);
                } else {
                    gtemp->params[a] = id;
                }
            }
            appendgatetocircuit(&(c.gateLL), &gtemp);
        } else if (gatetoken[0] == 'P') {
            gtemp->params = malloc(2*sizeof(int));
            gtemp->kind = PASS;
            int a, id;
            for(a = 0; a < 2; a++){
                fscanf(fp, " %16s", currentvar);
                id = getuniqueidfromvar(currentvar, &c);
                if (id == -1) {
                    Variable *tempvar = allocatevariable();
                    tempvar->name = strcpy(tempvar->name, currentvar);
                    tempvar->unique_id = next_unique_id;
                    gtemp->params[a] = tempvar->unique_id;
                    next_unique_id++;
                    tempvar->next = NULL;
                    if (strequals(currentvar, "0")) {
                        tempvar->value = 0;
                    } else if (strequals(currentvar, "1")) {
                        tempvar->value = 1;
                    }
                    appendvartocircuitlist(&(c.tempLL), &tempvar);
                } else {
                    gtemp->params[a] = id;
                }
            }
            appendgatetocircuit(&(c.gateLL), &gtemp);
        } else if (gatetoken[0] == 'D') {
            int inputsize = 0;
            fscanf(fp, " %d", &inputsize);
            int totalparamsize = inputsize + (1 << inputsize);
            gtemp->params = malloc(totalparamsize*sizeof(int));
            gtemp->kind = DECODER;
            gtemp->size = inputsize;
            int a, id;
            for(a = 0; a < totalparamsize; a++) {
                fscanf(fp, " %16s", currentvar);
                id = getuniqueidfromvar(currentvar, &c);
                if(id == -1){ 
                    Variable *tempvar = allocatevariable();
                    tempvar->name = strcpy(tempvar->name, currentvar);
                    tempvar->unique_id = next_unique_id;
                    gtemp->params[a] = tempvar->unique_id;
                    next_unique_id++;
                    tempvar->next = NULL;
                    if (strequals(currentvar, "0")) {
                        tempvar->value = 0;
                    } else if (strequals(currentvar, "1")) {
                        tempvar->value = 1;
                    }
                    appendvartocircuitlist(&(c.tempLL), &tempvar);
                } else {
                    gtemp->params[a] = id;
                }
            }
        appendgatetocircuit(&(c.gateLL), &gtemp);
        } else { //multiplexer
            int numselectors = 0;
            fscanf(fp, " %d", &numselectors);
            int nummuxinputs = 1 << numselectors;
            int paramsize = numselectors + nummuxinputs + 1;
            gtemp->params = malloc(paramsize*sizeof(int));
            gtemp->kind = MULTIPLEXER;
            gtemp->size = numselectors;
            int a, id;
            for(a = 0; a < paramsize; a++) {
                fscanf(fp, " %16s", currentvar);
                id = getuniqueidfromvar(currentvar, &c);
                if(id == -1){ 
                    Variable *tempvar = allocatevariable();
                    tempvar->name = strcpy(tempvar->name, currentvar);
                    tempvar->unique_id = next_unique_id;
                    gtemp->params[a] = tempvar->unique_id;
                    next_unique_id++;
                    tempvar->next = NULL;
                    if (strequals(currentvar, "0")) {
                        tempvar->value = 0;
                    } else if (strequals(currentvar, "1")) {
                        tempvar->value = 1;
                    }
                    appendvartocircuitlist(&(c.tempLL), &tempvar);
                } else {
                    gtemp->params[a] = id;
                }
            }
        appendgatetocircuit(&(c.gateLL), &gtemp);
        }
    }
    fclose(fp);
    // ^^^^ Circuit Read in Complete ^^^^ // 

    // LOOP for Prints

    int numrows = 1 << numinputs;
    int **truthtableinputs = generatetruthtableinputs(numinputs);

    int k, j;
    for(k = 0; k < numrows; k++) {
        //Set inputs
        Variable *current = c.inputLL;
        for(j = 0; j < numinputs; j++) {
            current->value = truthtableinputs[k][j];
            current = current->next;
            printf("%d ", truthtableinputs[k][j]);
        }

        // Enumerate through gates
        Gate *temp = c.gateLL;
        while(temp != NULL) {
            //Run gate functions
            switch(temp->kind) {
                case AND:
                    and(getvarfromuniqueid(temp->params[0], &c), getvarfromuniqueid(temp->params[1], &c), getvarfromuniqueid(temp->params[2], &c));
                    break;
                case OR:
                    or(getvarfromuniqueid(temp->params[0], &c), getvarfromuniqueid(temp->params[1], &c), getvarfromuniqueid(temp->params[2], &c));
                    break;
                case NAND:
                    nand(getvarfromuniqueid(temp->params[0], &c), getvarfromuniqueid(temp->params[1], &c), getvarfromuniqueid(temp->params[2], &c));
                    break;
                case NOR:
                    nor(getvarfromuniqueid(temp->params[0], &c), getvarfromuniqueid(temp->params[1], &c), getvarfromuniqueid(temp->params[2], &c));
                    break;
                case XOR:
                    xor(getvarfromuniqueid(temp->params[0], &c), getvarfromuniqueid(temp->params[1], &c), getvarfromuniqueid(temp->params[2], &c));
                    break;
                case NOT:
                    not(getvarfromuniqueid(temp->params[0], &c), getvarfromuniqueid(temp->params[1], &c));                
                    break;
                case PASS:
                    pass(getvarfromuniqueid(temp->params[0], &c), getvarfromuniqueid(temp->params[1], &c));
                    break;
                case DECODER:
                    decoder(temp->size, temp->params, &c);
                    break;
                case MULTIPLEXER:
                    multiplexer(temp->size, temp->params, &c);
                    break;
                default:
                    puts("error in switch incorrect input to gate type");
                    return EXIT_FAILURE;
            }
            temp = temp->next;
        }
        printf("|");

        //print outputs
        Variable *outtemp = c.outputLL;
        while(outtemp != NULL) {
            printf(" %d", outtemp->value);
            outtemp = outtemp->next;
        }
        puts("");
    }


    // Free Resources

    freematrix(truthtableinputs, numrows);

    //Free a Circuits LL
        // TODO: Free gates
    Variable *current = c.inputLL;
    Variable *last = NULL;

    while(current != NULL) {
        last = current;
        current = current->next;
        freevariable(last);
    }


    current = c.outputLL;
    last = NULL;

    while(current != NULL) {
        last = current;
        current = current->next;
        freevariable(last);
    }

    current = c.tempLL;
    last = NULL;

    while(current != NULL) {
        last = current;
        current = current->next;
        freevariable(last);
    }

    Gate *currentgate = c.gateLL;
    Gate *lastgate = NULL;
    while(currentgate != NULL) {
        lastgate = currentgate;
        currentgate = currentgate->next;
        freegate(lastgate);
    }


    return EXIT_SUCCESS;
}

