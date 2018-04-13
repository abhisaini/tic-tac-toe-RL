#!/usr/bin/python
import numpy as np
import random as rand
import Weight_Learner as wl
WEIGHTS = {
    'win' : 0.0,
    'fork' : 0.0,
    'block' : 0.0,
    'center' : 0.0,
    'oppCprner' : 0.0,
    'emptyCorner' : 0.0,
    'emptySide' : 0.0,
    'oppForkBlock' : 0.0
    }
def change_weight(delta_weight, model) :
    WEIGHTS[model] = WEIGHTS[model] + delta_weight


def trainer(value, iteration_count) :
    count = 0
    delta_weight = value
    while count < iteration_count :
        print("count : ",count)
        print("WEIGHTS : ", WEIGHTS)
        change_weight(delta_weight, "win")
        prev_win_percent = wl.games(WEIGHTS)[0]
        print(prev_win_percent)
        next_win_percent = wl.games(WEIGHTS)[0]
        delta_win_percent = next_win_percent - prev_win_percent
        # change_weight(delta_win_percent/1000, "win")
        print("Delta Percent : ", delta_win_percent)
        if delta_win_percent > 0 :
            count = count + 1
            continue
        else :
            delta_weight = -1*delta_weight
            count = count + 1
            continue
    while count < iteration_count :
        print("count : ",count)
        prev_win_percent = wl.games(WEIGHTS)[0]
        change_weight(delta_weight, "block")
        next_win_percent = wl.games(WEIGHTS)[0]
        delta_win_percent = next_win_percent - prev_win_percent
        # change_weight(delta_win_percent/1000, "block")
        if delta_win_percent > 0 :
            count = count + 1
            continue
        else :
            delta_weight = -1*delta_weight
            continue

    while count < iteration_count :
        print("count : ",count)
        prev_win_percent = wl.games(WEIGHTS)[0]
        change_weight(delta_weight, "fork")
        next_win_percent = wl.games(WEIGHTS)[0]

        delta_win_percent = next_win_percent - prev_win_percent
        # change_weight(delta_win_percent/1000, "fork")
        if delta_win_percent > 0 :
            count = count + 1
            continue
        else :
            delta_weight = -1*delta_weight
            continue

    while count < iteration_count :
        prev_win_percent = wl.games(WEIGHTS)[0]
        change_weight(delta_weight, "oppForkBlock")
        next_win_percent = wl.games(WEIGHTS)[0]
        delta_win_percent = next_win_percent - prev_win_percent
        # change_weight(delta_win_percent/1000, "oppForkBlock")
        if delta_win_percent > 0 :
            count = count + 1
            continue
        else :
            delta_weight = -1*delta_weight
            continue

    while count < iteration_count :
        prev_win_percent = wl.games(WEIGHTS)[0]
        change_weight(delta_weight, "center")
        next_win_percent = wl.games(WEIGHTS)[0]
        delta_win_percent = next_win_percent - prev_win_percent
        # change_weight(delta_win_percent/1000, "center")
        if delta_win_percent > 0 :
            count = count + 1
            continue
        else :
            delta_weight = -1*delta_weight
            continue

    while count < iteration_count :
        prev_win_percent = wl.games(WEIGHTS)[0]
        change_weight(delta_weight, "oppCorner")
        next_win_percent = wl.games(WEIGHTS)[0]
        delta_win_percent = next_win_percent - prev_win_percent
        # change_weight(delta_win_percent/1000, "oppCorner")
        if delta_win_percent > 0 :
            count = count + 1
            continue
        else :
            delta_weight = -1*delta_weight
            continue

    while count < iteration_count :
        prev_win_percent = wl.games(WEIGHTS)[0]
        change_weight(delta_weight, "emptyCorner")
        next_win_percent = wl.games(WEIGHTS)[0]
        delta_win_percent = next_win_percent - prev_win_percent
        # change_weight(delta_win_percent/1000, "emptyCorner")
        if delta_win_percent > 0 :
            count = count + 1
            continue
        else :
            delta_weight = -1*delta_weight
            continue

    while count < iteration_count :
        prev_win_percent = wl.games(WEIGHTS)[0]
        change_weight(delta_weight, "emptySide")
        next_win_percent = wl.games(WEIGHTS)[0]
        delta_win_percent = next_win_percent - prev_win_percent
        # change_weight(delta_win_percent/1000, "emptySide")
        if delta_win_percent > 0 :
            count = count + 1
            continue
        else :
            delta_weight = -1*delta_weight
            continue
count = 0
trainer(0.0001,10000)
print(WEIGHTS)
