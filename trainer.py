#!/usr/bin/python
import numpy as np
import random as rand
import Weight_Learner.py

def change_weight(delta_weight, model) :
    WEIGHTS(model) = WEIGHTS(model) + delta_weight

def trainer(value, iteration_count) :
    count = 0
    delta_weight = value
    while count < iteration_count :
        prev_win_percent = games[0]
        change_weight(delta_weight, "win")
        next_win_percent = games[0]
        delta_win_percent = next_win_percent - prev_win_percent

        if delta_win_percent > 0 :
            count = count + 1
            continue
        else :
            delta_weight = -1*delta_weight
            continue

    while count < iteration_count :
        prev_win_percent = games[0]
        change_weight(delta_weight, "block")
        next_win_percent = games[0]
        delta_win_percent = next_win_percent - prev_win_percent

        if delta_win_percent > 0 :
            count = count + 1
            continue
        else :
            delta_weight = -1*delta_weight
            continue

    while count < iteration_count :
        prev_win_percent = games[0]
        change_weight(delta_weight, "fork")
        next_win_percent = games[0]
        delta_win_percent = next_win_percent - prev_win_percent

        if delta_win_percent > 0 :
            count = count + 1
            continue
        else :
            delta_weight = -1*delta_weight
            continue

    while count < iteration_count :
        prev_win_percent = games[0]
        change_weight(delta_weight, "oppForkBlock")
        next_win_percent = games[0]
        delta_win_percent = next_win_percent - prev_win_percent

        if delta_win_percent > 0 :
            count = count + 1
            continue
        else :
            delta_weight = -1*delta_weight
            continue

    while count < iteration_count :
        prev_win_percent = games[0]
        change_weight(delta_weight, "center")
        next_win_percent = games[0]
        delta_win_percent = next_win_percent - prev_win_percent

        if delta_win_percent > 0 :
            count = count + 1
            continue
        else :
            delta_weight = -1*delta_weight
            continue

    while count < iteration_count :
        prev_win_percent = games[0]
        change_weight(delta_weight, "oppCorner")
        next_win_percent = games[0]
        delta_win_percent = next_win_percent - prev_win_percent

        if delta_win_percent > 0 :
            count = count + 1
            continue
        else :
            delta_weight = -1*delta_weight
            continue

    while count < iteration_count :
        prev_win_percent = games[0]
        change_weight(delta_weight, "emptyCorner")
        next_win_percent = games[0]
        delta_win_percent = next_win_percent - prev_win_percent

        if delta_win_percent > 0 :
            count = count + 1
            continue
        else :
            delta_weight = -1*delta_weight
            continue

    while count < iteration_count :
        prev_win_percent = games[0]
        change_weight(delta_weight, "emptySide")
        next_win_percent = games[0]
        delta_win_percent = next_win_percent - prev_win_percent

        if delta_win_percent > 0 :
            count = count + 1
            continue
        else :
            delta_weight = -1*delta_weight
            continue
