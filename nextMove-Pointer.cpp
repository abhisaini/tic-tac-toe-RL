state* nextMove (state &currState, state &nextState, int policy, int player) {

	// float largestValue = 0;
	int gridSize = currState.mat.size();
  int modelNo = modelClassifier(currState, player);
  int turns = countTurns(currState);

  // state dummyState(gridSize);
	if (policy == GREEDY) {

    // for (int i = 0; i < gridSize; i++) {
		// 	for (int j = 0; j < gridSize; j++) {
		// 		if (currState.mat[i][j] == 0){
    //       // modelClassifier(currState);
		// 			equate(currState, nextState);
		// 			nextState.mat[i][j] = 1;
		// 			nextState.val = getVal(nextState);
		// 			if (nextState.val >= largestValue){
		// 				largestValue = nextState.val;
		// 				// equate(nextState, dummyState);
		// 			}
		// 		}
		// 	}
		// }

    state* stateptr = NULL;
    int temp = 0;

    for (int i = 0; i < models.size(); i++) {
  		if (models[i].config == modelNo && models[i].turnCount == turns) {
  			stateptr = &models[i].array[0];
        temp = 1;
        break;
  		}
  	}

    if (!temp) {
        state S1(3);
        randomMove(currState, S1, PLAYER_X);
  			model A(modelNo);
  			A.turnCount = turns;
  			models.push_back(A);
  			A.array.push_back(S1);
				stateptr = &models[models.size()-1].array[0];
    }

		// std::vector<int> array;
		// int count = 0;
		// for (int i = 0; i < gridSize; i++) {
		// 	for (int j = 0; j < gridSize; j++) {
		// 		if (currState.mat[i][j] == 0){
		// 			equate(currState, nextState);
		// 			nextState.mat[i][j] = 1;
		// 			nextState.val = getVal(nextState);
		// 			if (nextState.val == largestValue){
		// 				array.push_back(3*i + j);
		// 				count++;
		// 			}
		// 		}
		// 	}
		// }
		// int freespace[count];
		// for (int i = 0; i < count; i++){
		//     freespace[i] = array[i];
		// }
		// // equate(dummyState, nextState);
		// int move = randomInput(freespace, count);
    //     randomChanged(currState, nextState, move, PLAYER_X);
		// nextState.val = getVal(nextState);
	}

	else {
		randomMove(currState, nextState, player);
    int temp = 0;
    for (int i = 0; i < models.size(); i++) {
  		if (models[i].config == modelNo && models[i].turnCount == turns) {
  			for (int j = 0; j < models[i].array.size(); j++) {
          if (compareMatrices(currState, nextState)) {
              stateptr = &models[i].array[j];
              break;
          }
        }
        temp = 1;
        break;
  		}
	  }
    if (!temp) {
      state s1(3);
      equate(currState, s1);
      model A(modelNo);
      A.turnCount = turns;
      models.push_back(A);
      A.array.push_back(s1);
      stateptr = &models[models.size()-1].array[0];
   }
  }

	return
}
