/*
 * song.c
 *
 *  Created on: Apr 27, 2019
 *      Author: Evan Christensen
 */

#include "song.h"

void playSong(oi_t *sensor_data)  {
    unsigned char notes0[] = {66, 69, 71, 73, 73, 31, 71, 73, 76, 78, 71, 69, 69, 73, 73, 73};//low: 66, 69, 71, 73, 73  //high: 86, 86, 84, 86, 31, 81, 31, 81, 86, 91, 90, 86; low: 74, 74, 72, 74, 31, 69, 31, 69, 74, 79, 78, 74

    unsigned char dur0[] = {15, 15, 15, 15, 15, 67, 15, 15, 15, 15, 15, 30, 15, 15, 15, 30};

    unsigned char notes1[] = {31, 69, 71, 73, 76, 78, 71, 69, 71, 73, 73};

    unsigned char dur1[] = {60, 15, 15, 15, 15, 15, 15, 30, 15, 15, 30};

    unsigned char notes2[] = {31, 69, 71, 73, 76, 78, 73, 71, 69, 71, 69, 31};

    unsigned char dur2[] = {60, 15, 15, 15, 15, 15, 15, 30, 15, 15, 120, 15};

    unsigned char notes3[] = {66, 31, 69, 31, 71, 31, 73, 74, 73, 31, 73, 74, 73, 71, 69, 71};

    unsigned char dur3[] = {15, 15, 15, 15, 15, 30, 30, 15, 22, 60, 15, 15, 15, 15, 30, 30};

    unsigned char notes4[] = {73, 74, 73, 31, 73, 74, 73, 71, 69, 71};

    unsigned char dur4[] = {30, 15, 22, 60, 15, 15, 15, 15, 30, 30};

    unsigned char notes5[] = {73, 74, 73, 31, 73, 74, 73, 71, 69, 66, 71, 69};

    unsigned char dur5[] = {30, 15, 22, 60, 15, 15, 15, 15, 30, 15, 15, 120};

    oi_loadSong(0, 16, notes0, dur0);

    oi_play_song(0);

    oi_update(sensor_data);

    while (sensor_data->songPlaying == 1)  {
        oi_update(sensor_data);
    }

    oi_loadSong(1, 11, notes1, dur1);

    oi_play_song(1);

    oi_update(sensor_data);

    while (sensor_data->songPlaying == 1)  {
        oi_update(sensor_data);
    }

    oi_loadSong(2, 12, notes2, dur2);

    oi_play_song(2);

    oi_update(sensor_data);

    while (sensor_data->songPlaying == 1)  {
        oi_update(sensor_data);
    }

    oi_loadSong(3, 16, notes3, dur3);

    oi_play_song(3);

    oi_update(sensor_data);

    while (sensor_data->songPlaying == 1)  {
        oi_update(sensor_data);
    }

    while (sensor_data->songPlaying == 1)  {
        oi_update(sensor_data);
    }

    oi_loadSong(0, 10, notes4, dur4);

    oi_play_song(0);

    oi_update(sensor_data);

    while (sensor_data->songPlaying == 1)  {
        oi_update(sensor_data);
    }

    oi_loadSong(1, 12, notes5, dur5);

    oi_play_song(1);

    oi_update(sensor_data);

    while (sensor_data->songPlaying == 1)  {
        oi_update(sensor_data);
    }
}

void playSong2(oi_t *sensor_data)
{
    unsigned char notes[] = { 86, 86, 84, 86, 31, 81, 31, 81, 86, 91, 90, 86 }; //high: 86, 86, 84, 86, 31, 81, 31, 81, 86, 91, 90, 86; low: 74, 74, 72, 74, 31, 69, 31, 69, 74, 79, 78, 74

    unsigned char dur[] = { 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15 };

    oi_loadSong(0, 12, notes, dur);
    oi_play_song(0);
}

