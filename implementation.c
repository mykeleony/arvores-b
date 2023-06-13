/*********************************************************************
 * Myke Leony dos Santos Amorim
 *
 *
 * Primeiro semestre de 2023
 *
 * Parte 1 do EP 2 de AED-II: árvores B sem índices
 *
 * Código de implementação de operações básicas em árvores B funcional, documentado e com código fonte original alterado.
 *********************************************************************/

#include "headers.h"

/**
	* @brief Cria ou carrega uma arvore B. \n
    *
	* Se mode for false, cria uma árvore com o nome do arquivo fornecido. \n
	* Se mode for true, carrega a árvore a partir do arquivo "tree.dat".
    *
    * @param fileName: Nome do arquivo de armazenagem da árvore B
    * @param mode: Indica se a árvore será criada ou carregada do arquivo
    *
	* @return Um ponteiro para a árvore B criada ou carregada.
    *
	* @attention Modificações do código original: foram adicionados tratamentos de erro nos trechos que manipulam arquivos.
*/
bTree* createTree(char* fileName, bool mode) {
	bTree* tree = malloc(sizeof(bTree));

	if (tree == NULL) {
		printf("Erro de alocacao de memoria.\n");
		return NULL;
	}

	// Novo arquivo
	if (!mode) {
		strcpy(tree->fileName, fileName);

		tree->fp = fopen(fileName, "w");

		if (tree->fp == NULL) {
			printf("Erro ao criar o arquivo.\n");
			free(tree);
			return NULL;
		}

		fclose(tree->fp);

		tree->root = 0;
		tree->nextPos = 0;
	}

	// Carrega arvore existente
	else {
		FILE* oldFile = fopen("tree.dat", "r");

		if (oldFile == NULL) {
			printf("Erro ao abrir o arquivo existente.\n");
			free(tree);
			return NULL;
		}

		fread(tree, sizeof(bTree), 1, oldFile);
		fclose(oldFile);
	}

	tree->fp = fopen(fileName, "r+");

	if (tree->fp == NULL) {
		printf("Erro ao abrir o arquivo.\n");
		free(tree);
		return NULL;
	}

	return tree;
}

/**
    * @brief Cria um no da árvore B.
    *
    * @param node: ponteiro para o nó a ser inicializado.
    * @param isLeaf: indica se o nó é uma folha ou não.
    * @param tree: o ponteiro para a árvore B à qual o nó pertencerá.
    *
    * @return o ponteiro para o nó inicializado.
    *
    * @attention Modificações do código original: declaração redundante do iterador 'i' dentro do laço for foi removida.
*/
bTreeNode* nodeInit(bTreeNode* node, bool isLeaf, bTree* tree) {
	node->isLeaf = isLeaf;
	node->noOfRecs = 0;
	node->pos = tree->nextPos;
	(tree->nextPos)++;

	int i;

	for (i = 0; i < (2 * t); ++i) {
		node->children[i] = -1;
	}

	return node;
}

/**
	* @brief Escreve um nó de árvore B no arquivo associado à árvore.
    *
	* @param ptr_tree: ponteiro para a árvore B.
	* @param p: ponteiro para o nó da árvore B a ser escrito.
	* @param pos: posição onde o nó deve ser escrito no arquivo. Se for -1, o nó será escrito na próxima posição disponível.
    *
	* @return void.
    *
	* @attention Modificações do código original: terceiro argumento de fseek alterado de '0' para 'SEEK_SET' para garantir que ptr_tree e o arquivo estejam sincronizados sobre a posição atual.
*/
void writeFile(bTree* ptr_tree, bTreeNode* p, int pos) {
	if(pos == -1) {
		pos = ptr_tree->nextPos++;
	}

	fseek(ptr_tree->fp, pos * sizeof(bTreeNode), SEEK_SET);
	fwrite(p, sizeof(bTreeNode), 1, ptr_tree->fp);
}


/**
	* @brief Lê um nó de uma árvore B a partir de uma posição específica no arquivo.
    *
	* @param ptr_tree: Ponteiro para a estrutura da árvore B.
	* @param p: Ponteiro para o nó a ser preenchido com os dados lidos.
	* @param pos: Posição do nó no arquivo.
    *
	* @return void.
    *
	* @attention Modificações do código original: nenhuma modificação no código original foi necessária.
*/
void readFile(bTree* ptr_tree, bTreeNode* p, int pos) {
  fseek(ptr_tree->fp, pos * sizeof(bTreeNode), SEEK_SET);
  fread(p, sizeof(bTreeNode), 1, ptr_tree->fp);
}

/**
	* @brief Insere dados em um nó de registro de uma árvore B, assumindo que os parâmetros do tipo char[] possuem espaço suficiente para armazenar os valores atribuídos.
    *
	* @param record: ponteiro para o nó de registro onde os dados serão inseridos
	* @param id_num: número de identificação a ser atribuído ao registro
	* @param country: país a ser atribuído ao registro
	* @param grate: avaliação a ser atribuída ao registro
	* @param score: pontuação a ser atribuída ao registro
	* @param rate: taxa a ser atribuída ao registro
    *
	* @return void.
    *
	* @attention Modificações do código original: removido retorno redundante na última linha.
*/
void enterData(recordNode* record, int id_num, char country[], char grate[], int score, int rate) {
	record->key = id_num;
	strcpy(record->country, country);
	strcpy(record->grate, grate);
	record->score = score;
	record->rate = rate;
}

/**
	* @brief Obtem dados de um arquivo e os armazena em uma estrutura de árvore B.
    *
	* @param filepath: caminho do arquivo a ser lido
	* @param len: número máximo de registros a serem lidos do arquivo
    *
	* @return um ponteiro para o vetor de registros preenchido com os dados lidos do arquivo.
    *
	* @attention Modificações do código original: nenhuma modificação no código original foi necessária.
*/
recordNode* getData(char *filepath, int len) {
    recordNode *recordArr = malloc(sizeof(recordNode) * len);

    char delim = ',';		// Registros devem estar separados por vírgula
    char line[256];

    int file_no = 0; // Contador de registros lidos do arquivo
    int i, key, score, rate;

    char country[255];
    char grate[255];

    FILE *inpFile = fopen(filepath, "r");

    while((file_no < len) && fscanf(inpFile, "%d,%[^,],%[^,],%d,%d", &key, country, grate, &score, &rate)) {
      enterData(&recordArr[file_no], key, country, grate, score, rate);
      file_no++;
    }

    return recordArr;
}

/**
 * @brief Divide um filho de um nó da árvore B.
 *
 * @param tree: ponteiro para a estrutura da árvore B
 * @param x: nó pai do filho a ser dividido
 * @param i: posição do filho a ser dividido
 * @param y: nó filho a ser dividido
 *
 * @return void.
 *
 * @attention Modificações do código original: nenhuma modificação no código original foi necessária.
 */
void splitChild(bTree* tree, bTreeNode* x, int i, bTreeNode* y) {
	bTreeNode* z = malloc(sizeof(bTreeNode));
	nodeInit(z, y->isLeaf, tree);
	z->noOfRecs = t - 1;

	int j;

    // Copia os registros da metade superior do nó y para o novo nó z
	for(j = 0; j < (t - 1); j++) {
		z->recordArr[j] = y->recordArr[j + t];
	}

	if(!y->isLeaf) {
        // Copia os ponteiros dos filhos da metade superior do nó y para o novo nó z
		for(j = 0; j < t; j++) {
			z->children[j] = y->children[j + t];
            y->children[j + t] = -1;
		}
	}

	y->noOfRecs = t - 1;

    // Desloca os ponteiros dos filhos do nó x para abrir espaço para o novo nó z
	for(j = x->noOfRecs; j >= i + 1; j--) {
		x->children[j + 1] = x->children[j];
	}

	x->children[i + 1] = z->pos;

    // Desloca as chaves do nó x para abrir espaço para a chave mediana
	for(j = x->noOfRecs - 1; j >= i; j--) {
		x->recordArr[j + 1] = x->recordArr[j];
	}

    // Insere a chave mediana no nó x
	x->recordArr[i] = y->recordArr[t - 1];
	x->noOfRecs++;

    writeFile(tree, x, x->pos);
    writeFile(tree, y, y->pos);
    writeFile(tree, z, z->pos);

	free(z);
}

/**
 * @brief Insere um registro em um nó não cheio de uma árvore B.
 *
 * @param tree: ponteiro para a estrutura da árvore B.
 * @param x: nó onde será feita a inserção.
 * @param record: registro a ser inserido.
 *
 * @return void.
 *
 * @attention Modificações do código original: nenhuma modificação no código original foi necessária.
 */
void insertNonFull(bTree* tree, bTreeNode* x, recordNode* record) {
	int i = x->noOfRecs - 1;

	if (x->isLeaf == true) {
        // Insere o novo registro no nó folha x mantendo a ordenação
		while (i >= 0 && record->key < x->recordArr[i]->key) {
            x->recordArr[i + 1] = x->recordArr[i];
			i--;
		}

		x->recordArr[i + 1] = record;
		x->noOfRecs++;

        writeFile(tree, x, x->pos);
	}

	else {
        // Procura pelo filho apropriado para inserção
		while (i >= 0 && record->key < x->recordArr[i]->key) {
			i--;
		}

		bTreeNode* childAtPosi = malloc(sizeof(bTreeNode));

        readFile(tree, childAtPosi, x->children[i + 1]);

		if (childAtPosi->noOfRecs == (2 * t - 1)) {
            // Filho está cheio e precisa ser dividido
			splitChild(tree,x,i + 1,childAtPosi);

			if (x->recordArr[i + 1]->key < record->key) {
				i++;
			}
		}

        readFile(tree, childAtPosi, x->children[i + 1]);
		insertNonFull(tree,childAtPosi,record);

		free(childAtPosi);
	}
}

/**
    * @brief Insere um novo nó de registro em uma árvore B.
    *
	* @param tree: árvore B a ter registro inserido.
	* @param record: registro a ser inserido.
    *
	* @return void.
    *
	* @attention Modificações do código original: nenhuma modificação no código original foi necessária.
*/
void insert(bTree* tree, recordNode* record) {
	// Árvore está vazia. Novo nó será raiz.
	if(tree->nextPos == 0) {
		tree->root = tree->nextPos;

		bTreeNode* firstNode = malloc(sizeof(bTreeNode));
		nodeInit(firstNode,true,tree);
		firstNode->recordArr[0] = record;
		(firstNode->noOfRecs)++;

    writeFile(tree, firstNode, firstNode->pos);

		free(firstNode);

		return;
	}

	else {
		bTreeNode* rootCopy = malloc(sizeof(bTreeNode));

    readFile(tree, rootCopy, tree->root);

		// Nó raiz está cheio. Necessário fazer a divisão.
		if(rootCopy->noOfRecs == (2 * t - 1)) {
			bTreeNode* newRoot = malloc(sizeof(bTreeNode));

			nodeInit(newRoot, false, tree);

			newRoot->children[0] = tree->root;

			splitChild(tree, newRoot, 0, rootCopy);

			int i = 0;

			if (newRoot->recordArr[0]->key < record->key) {
				i++;
			}

			bTreeNode* childAtPosi = malloc(sizeof(bTreeNode));

      readFile(tree, childAtPosi, newRoot->children[i]);
			insertNonFull(tree,childAtPosi,record);

			tree->root = newRoot->pos;

			free(newRoot);
      free(childAtPosi);
		}

		// Inserção diretamente na raiz não cheia.
		else {
			insertNonFull(tree,rootCopy,record);
		}

		free(rootCopy);
	}
}

/**
	* @brief Percorre em ordem e recursivamente uma árvore B imprimindo cada nó visitado.
    *
	* @param tree: ponteiro para a estrutura da árvore B.
	* @param root: índice da raiz do nó a ser percorrido.
    *
	* @return void.
    *
	* @attention Modificações do código original: declaração do iterador 'i' foi deslocada de dentro para fora do for.
*/
void traverse(bTree* tree, int root) {
	if (root == -1) {
		return;
	}

	bTreeNode *toPrint = malloc(sizeof(bTreeNode));

	readFile(tree, toPrint, root);
	dispNode(toPrint);

	int i;

	for (i = 0; i < (2 * t); i++) {
		traverse(tree, toPrint->children[i]);
	}

	free(toPrint);
}

/**
	* @brief Imprime informações de um nó da árvore B.
    *
	* @param node: ponteiro para o nó a ser impresso em tela.
    *
	* @return void.
    *
	* @attention Modificações do código original:
		- Declaração do iterador 'i' foi deslocada de dentro para fora dos laços for;
		- printf duplicado na última linha foi unificado em um.
*/
void dispNode(bTreeNode* node) {
	printf("Position in node:%d\n", node->pos );
  printf("Number of Records:%d\n", node->noOfRecs );
	printf("Is leaf?:%d\n", node->isLeaf);
	printf("Keys:\n");

	int i;

	for (i = 0; i < node->noOfRecs; i++) {
		printf("%d ", node->recordArr[i]->key);
	}

	printf("\n");
	printf("Links:\n");

	for (i = 0; i < (2 * t); ++i) {
		printf("%d ",node->children[i]);
	}

	printf("\n\n");
}


/**
	* Busca recursivamente e a partir de um nó arbitrário um registro com uma dada chave em uma árvore B.
    *
	* tree: Ponteiro para a estrutura da árvore B.
	* key: Chave do registro a ser encontrado.
	* root: Ponteiro para o nó origem da árvore.
    *
	* @return Ponteiro para o registro encontrado com a chave especificada caso exista ou NULL caso contrário.
    *
	* @attention Modificações do código original: nenhuma modificação no código original foi necessária.
*/
recordNode* searchRecursive(bTree* tree, int key, bTreeNode* root) {
	int i = 0;

	// Busca a posição correta para buscar o registro no nó atual
	while (i < root->noOfRecs && key > root->recordArr[i]->key) {
		i++;
	}

	if (i < root->noOfRecs && key == root->recordArr[i]->key) {
		return root->recordArr[i];	// Registro foi encontrado
	}

	else if (root->isLeaf) {
		return NULL;	 // Nó atual é folha: o registro não existe na árvore
	}

	else {
		bTreeNode* childAtPosi = malloc(sizeof(bTreeNode));

		readFile(tree, childAtPosi, root->children[i]);	// Lê o filho correspondente à posição encontrada e continua a busca recursiva nele

		recordNode* found = searchRecursive(tree, key, childAtPosi);
		free(childAtPosi);
		return found;
	}
}

/**
	* @brief Busca o registro de uma árvore B a partir de uma chave de busca.
    *
	* @param tree: ponteiro para a estrutura da árvore B.
	* @param key: valor da chave a ser encontrada na árvore.
    *
    * @return o ponteiro para o nó que contém o valor buscado ou NULL caso o valor não seja encontrado.
    *
	* @attention Modificações do código original: nenhuma modificação no código original foi necessária.
*/
recordNode* search(bTree* tree, int key) {
	bTreeNode* root = malloc(sizeof(bTreeNode));

	readFile(tree, root, tree->root);

	recordNode* result = searchRecursive(tree, key, root);

	free(root);

	return result;
}

/**
 * @brief Encontra a posição da chave em um nó da árvore B.
 *
 * @param node: nó que contém a chave
 * @param k: valor da chave a ser encontrada
 *
 * @return o índice da chave no nó.
 *
 * @attention Modificações do código original: nenhuma modificação no código original foi necessária.
 */
int findKey(bTreeNode* node, int k) {
    int idx = 0;

    while (idx < node->noOfRecs && node->recordArr[idx]->key < k) {
        ++idx;
    }

    return idx;
}

/**
 * @brief Remove um registro de um nó folha de uma árvore B.
 *
 * @param tree: ponteiro para a árvore B
 * @param node: nó folha do qual o registro será removido
 * @param idx: índice do registro no vetor de registros de node
 *
 * @return void.
 *
 * @attention Modificações do código original: nenhuma modificação no código original foi necessária.
 */
void removeFromLeaf (bTree* tree, bTreeNode *node, int idx) {
    int i;

    // Movimenta os registros seguintes para a esquerda para preencher o espaço do registro removido
    for (i = (idx + 1); i < node->noOfRecs; ++i) {
	    node->recordArr[i - 1] = node->recordArr[i];
    }

    node->noOfRecs--;
}

/**
 * @brief Remove um registro de um nó não folha em uma árvore B.
 *
 * @param tree: ponteiro para a árvore B
 * @param node: nó não folha do qual o registro será removido
 * @param idx: índice do registro no vetor de registros de node
 *
 * @return void.
 *
 * @attention Modificações do código original: nenhuma modificação no código original foi necessária.
 */
void removeFromNonLeaf(bTree* tree, bTreeNode *node, int idx) {
    int k = node->recordArr[idx]->key;

    bTreeNode *child = malloc(sizeof(bTreeNode));
    bTreeNode *sibling = malloc(sizeof(bTreeNode));

    readFile(tree, child, node->children[idx]);
    readFile(tree, sibling, node->children[idx+1]);

    // Caso 2.a: filho x possui ao menos t chaves
    if (child->noOfRecs >= t) {
        recordNode* pred = getPred(tree, node, idx);
        node->recordArr[idx] = pred;
        removeNode(tree, child, pred->key);
    }

    // Caso 2.b: irmão y possui ao menos t chaves
    else if (sibling->noOfRecs >= t) {
        recordNode* succ = getSucc(tree, node, idx);
        node->recordArr[idx] = succ;
        removeNode(tree, sibling, succ->key);
    }

    // Caso 2.c: ambos x e y possuem t-1 chaves
    else {
        child = merge(tree, node, idx);
        removeNode(tree, child, k);

	    return;
    }

    writeFile(tree, child, child->pos);
    writeFile(tree, sibling, sibling->pos);

    free(child);
    free(sibling);
}

/**
 * @brief Remove um nó de uma árvore B.
 *
 * @param tree: ponteiro para a estrutura da árvore B
 * @param node: nó a ser removido
 * @param k: valor da chave do registro a ser removido
 *
 * @return void.
 *
 * @attention Modificações do código original: cenário de chave inexistente @returnva "false", mas a função é void.
 */
void removeNode(bTree* tree, bTreeNode* node, int k) {
    int idx = findKey(node, k);

    // Verifica se a chave a ser removida está presente no nó
    if (idx < node->noOfRecs && node->recordArr[idx]->key == k) {
        if (node->isLeaf){
	        removeFromLeaf(tree, node, idx);
        } else {
            removeFromNonLeaf(tree, node, idx);
        }

	    writeFile(tree, node, node->pos);
    }

    else {
        if (node->isLeaf) {
            // Chave não existe
		    return;
       	}

        bool flag = (idx == node->noOfRecs);

        bTreeNode *childAtPosi = malloc(sizeof(bTreeNode));
        readFile(tree, childAtPosi, node->children[idx]);

        if (childAtPosi->noOfRecs < t) {
            fill(tree, node, idx);  // Preenche o nó filho com registros de outros nós
            readFile(tree, childAtPosi, node->children[idx]);
        }

        if (flag && idx > node->noOfRecs) {
            bTreeNode *sibling = malloc(sizeof(bTreeNode));
            readFile(tree, sibling, node->children[idx - 1]);
            removeNode(tree, sibling, k);

            writeFile(tree, sibling, sibling->pos); // Atualiza o irmão esquerdo no arquivo
            free(sibling);
        }
        else {
            removeNode(tree, childAtPosi, k);   // Remove o registro do nó filho
        }

        writeFile(tree, childAtPosi, childAtPosi->pos);
        free(childAtPosi);
    }
}

/**
 * @brief Obtém o predecessor de um nó em uma árvore B.
 *
 * @param tree: ponteiro para a estrutura da árvore B
 * @param node: nó em que o predecessor será procurado
 * @param idx: índice do nó no vetor de filhos do pai
 *
 * @return o registro do predecessor encontrado.
 *
 * @attention Modificações do código original: nenhuma modificação no código original foi necessária.
 */
recordNode* getPred(bTree* tree, bTreeNode *node, int idx) {
    bTreeNode *curr = malloc(sizeof(bTreeNode));

    readFile(tree, curr, node->children[idx]);

    while (!curr->isLeaf) {
        readFile(tree, curr, curr->children[curr->noOfRecs]);
    }

    recordNode* result = curr->recordArr[curr->noOfRecs - 1];

    free(curr);

    return result;
}


/**
 * @brief Obtém o sucessor de um nó em uma árvore B.
 *
 * @param tree: ponteiro para a estrutura da árvore B
 * @param node: nó em que o sucessor será procurado
 * @param idx: índice do nó no vetor de filhos do pai
 *
 * @return o registro do sucessor encontrado.
 *
 * @attention Modificações do código original: nenhuma modificação no código original foi necessária.
 */
recordNode* getSucc(bTree* tree, bTreeNode *node, int idx) {
    bTreeNode *curr = malloc(sizeof(bTreeNode));

    readFile(tree, curr, node->children[idx+1]);

    while (!curr->isLeaf) {
        readFile(tree, curr, curr->children[0]);
    }

    recordNode* result = curr->recordArr[0];
    free(curr);

    return result;
}


/**
 * @brief Preenche um nó da árvore B com registros
 *
 * @param tree: ponteiro para a estrutura da árvore B
 * @param node: nó a ser preenchido
 * @param idx: índice do nó no vetor de filhos do nó pai
 *
 * @returns void
 *
 * @attention Modificações do código original: nenhuma modificação no código original foi necessária.
 *
 */
void fill(bTree* tree, bTreeNode *node, int idx) {
    bTreeNode *cPrev = malloc(sizeof(bTreeNode));
    bTreeNode *cSucc = malloc(sizeof(bTreeNode));

    readFile(tree, cPrev, node->children[idx - 1]);
    readFile(tree, cSucc, node->children[idx + 1]);

    if (idx != 0 && cPrev->noOfRecs >= t) {
        borrowFromPrev(tree, node, idx);
    }

    else if (idx != node->noOfRecs && cSucc->noOfRecs >= t) {
        borrowFromNext(tree, node, idx);
    }

    // Se não é possível emprestar do irmão anterior ou próximo, realiza a fusão com os nós vizinhos
    else {
        if (idx != node->noOfRecs) {
            merge(tree, node, idx);
        } else {
            merge(tree, node, idx-1);
        }
    }

    free(cPrev);
    free(cSucc);
}


/**
    * @brief Realiza o empréstimo de um registro de um nó irmão (sibling) anterior para outro nó (node) em uma árvore B.
    *
    * @param tree: ponteiro para a estrutura da árvore B.
    * @param node: ponteiro para o nó atual.
    * @param idx: índice do registro que precisa ser emprestado.
    *
    * @attention Modificações do código original: \n
        - retorno redudante da última linha foi removido; \n
        - declarações do iterador 'i' foram unificados em uma única declaração fora do for; \n
        - incrementos e decrementos sobre quantidade de registros dos nós simplificados para os operadores ++ e --; \n
        - foram adicionados tratamentos de erro.
*/
void borrowFromPrev(bTree* tree, bTreeNode *node, int idx) {
    bTreeNode *child = malloc(sizeof(bTreeNode));

    if (child == NULL) {
        return;
    }

    bTreeNode *sibling = malloc(sizeof(bTreeNode));

    if (sibling == NULL) {
        free(child);
        return;
    }

    // Índice inválido
    if (idx <= 0 || idx > node->noOfRecs) {
        free(child);
        free(sibling);

        return;
    }

    readFile(tree, child, node->children[idx]);
    readFile(tree, sibling, node->children[idx-1]);

    int i;

    // Desloca os registros do nó filho para a direita para liberar espaço para o novo registro
    for (i = child->noOfRecs - 1; i >= 0; --i) {
        child->recordArr[i + 1] = child->recordArr[i];
    }

    // Se o nó child não é uma folha, desloca os ponteiros dos filhos para a direita
    if (!child->isLeaf) {
        for (i = child->noOfRecs; i >= 0; --i) {
            child->children[i + 1] = child->children[i];
        }
    }

    child->recordArr[0] = node->recordArr[idx - 1];

    // Se o nó atual não é uma folha, atualiza o ponteiro do primeiro filho do nó filho
    if (!node->isLeaf) {
        child->children[0] = sibling->children[sibling->noOfRecs];
        sibling->children[sibling->noOfRecs] = -1;
    }

    node->recordArr[idx - 1] = sibling->recordArr[sibling->noOfRecs-1];

    child->noOfRecs++;
    sibling->noOfRecs--;

    writeFile(tree, node, node->pos);
    writeFile(tree, child, child->pos);
    writeFile(tree, sibling, sibling->pos);

    free(child);
    free(sibling);
}


/**
    * @brief Realiza o empréstimo de um registro de um nó irmão (sibling) para outro nó (node) em uma árvore B.
    *
    * @param tree: ponteiro para a estrutura da árvore B.
    * @param node: ponteiro para o nó atual.
    * @param idx: índice do registro no nó atual que precisa ser substituído pelo registro emprestado.
    *
    * @attention Modificações do código original: \n
        - retorno redudante da última linha foi removido; \n
        - declarações do iterador 'i' foram unificados em uma única declaração fora do for; \n
        - incrementos e decrementos sobre quantidade de registros dos nós simplificados para os operadores ++ e --
*/
void borrowFromNext(bTree* tree, bTreeNode *node, int idx) {
    bTreeNode *child = malloc(sizeof(bTreeNode));
    bTreeNode *sibling = malloc(sizeof(bTreeNode));

    readFile(tree, child, node->children[idx]);
    readFile(tree, sibling, node->children[idx + 1]);

    child->recordArr[(child->noOfRecs)] = node->recordArr[idx]; // Substitui o registro no nó filho pelo registro no nó atual.

    if (!(child->isLeaf)) {
        child->children[(child->noOfRecs) + 1] = sibling->children[0];  // Atualiza o ponteiro para o filho no nó filho.
    }

    node->recordArr[idx] = sibling->recordArr[0];

    int i;

    for (i = 1; i < sibling->noOfRecs; ++i) {
        sibling->recordArr[i - 1] = sibling->recordArr[i]; // Desloca os registros no próximo irmão.
    }

    if (!sibling->isLeaf) {
        for (i = 1; i <= sibling->noOfRecs; ++i) {
            sibling->children[i - 1] = sibling->children[i];    // Desloca os filhos no próximo irmão.
        }

        sibling->children[sibling->noOfRecs] = -1; // Remove o último ponteiro para filho no próximo irmão para evitar potenciais problemas.
    }

    child->noOfRecs++;
    sibling->noOfRecs--;

    writeFile(tree, node, node->pos);
    writeFile(tree, child, child->pos);
    writeFile(tree, sibling, sibling->pos);

    free(child);
    free(sibling);
}

/**
    * @brief Realiza a fusão (merge) de um nó filho e um nó irmão de uma árvore B.
    *
    * @param tree: ponteiro para a estrutura da árvore B
    * @param node: ponteiro para o nó pai
    * @param idx: índice do nó filho no nó pai
    *
    * @return o ponteiro para nó fundido.
    *
    * @attention Modificações do código original: declarações do iterador 'i' foram unificados em uma única declaração fora do for.
*/
bTreeNode* merge(bTree* tree, bTreeNode *node, int idx) {
    bTreeNode *child = malloc(sizeof(bTreeNode));
    bTreeNode *sibling = malloc(sizeof(bTreeNode));

    // Lê o conteúdo do nó filho e do nó irmão do arquivo
    readFile(tree, child, node->children[idx]);
    readFile(tree, sibling, node->children[idx+1]);

    // Move o registro correspondente do nó pai para o nó filho
    child->recordArr[t - 1] = node->recordArr[idx];

    int i;

    // Copia os registros do nó irmão para o nó filho
    for (i = 0; i < sibling->noOfRecs; ++i)
        child->recordArr[i + t] = sibling->recordArr[i];

    // Atualiza os ponteiros dos filhos, se existirem
    if (!child->isLeaf) {
        for(i = 0; i <= sibling->noOfRecs; ++i)
            child->children[i + t] = sibling->children[i];
    }

    // Move os registros e os ponteiros do nó pai
    for (i = (idx + 1); i < node->noOfRecs; ++i) {
        node->recordArr[i - 1] = node->recordArr[i];
    }

    for (i = (idx + 2); i <= node->noOfRecs; ++i) {
        node->children[i - 1] = node->children[i];
    }

    node->children[node->noOfRecs] = -1;
    child->noOfRecs += sibling->noOfRecs + 1;
    node->noOfRecs--;

    // Se o nó pai ficar vazio após o merge, atualiza o ponteiro da raiz
    if(node->noOfRecs == 0) {
        tree->root = node->children[0];
    }

    // Escreve as alterações no arquivo
    writeFile(tree, node, node->pos);
    writeFile(tree, child, child->pos);
    writeFile(tree, sibling, sibling->pos);

    free(sibling);

    return child;
}

/**
    * @brief Remove o registro com a chave especificada da árvore B.
    *
    * @param tree: ponteiro para a estrutura de árvore B
    * @param key: valor da chave do registro a ser removido
    *
    * @return um booleano indicando se a remoção foi bem-sucedida.
    *
	* @attention Modificações do código original: \n
        - A linha "if(found);" estava seguida por um ponto e vírgula, fazendo com que a remoção fosse executada
        independentemente da existência do registro. O ponto e vírgula foi removido.
*/
bool removeFromTree(bTree* tree, int key) {
    bTreeNode *root = malloc(sizeof(bTreeNode));

    readFile(tree, root, tree->root);

    bool found = search(tree, key);

    if(found) {
        removeNode(tree, root, key);
    }

    free(root);

    return found;
}

/**
 * @brief Imprime detalhes dos nós, por ordem de alocação, da árvore B a partir do arquivo.
 *
 * @param tree: ponteiro para estrutura da árvore B.
 *
 * @return void.
 *
 * @attention Modificações do código original: declaração do iterador 'i' deslocado de dentro para fora do laço for.
 */
void hardPrint(bTree* tree) {
    bTreeNode* lido = (bTreeNode*) malloc(sizeof(bTreeNode));

    int i;

    for(i = 0; i < tree->nextPos; i++) {
        fseek(tree->fp, i * sizeof(bTreeNode), SEEK_SET);
        fread(lido, sizeof(bTreeNode), 1, tree->fp);

        if(lido->isLeaf <= 1) {
            dispNode(lido);
        }

        else {
            printf("ERRO: isLeaf = %i\n\n", lido->isLeaf);
        }
    }

    free(lido);
}

/**
 * @brief Imprime uma árvore B de duas maneiras diferentes: \n
 *  - primeiro, do nó raiz aos nós filhos, seguindo a hierarquia da árvore; \n
 *  - depois, por ordem de alocação dos nós no arquivo. \n
 *
 * @param tree: ponteiro para a estrutura da árvore B
 *
 * @return void.
 *
 * @attention Modificações do código original: nenhuma modificação no código original foi necessária.
 */
void doublePrint(bTree* tree) {
    printf("=================");
    printf("\nTraverse\n");
    traverse(tree, tree->root);

    printf("=================");
    printf("\nHard print\n");
    hardPrint(tree);
}
