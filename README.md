# Find Path

## Problem Description

Consider a highway, which can be represented as a sequence of service stations. Each service station is uniquely identified by its distance from the starting point of the highway, expressed in kilometers as a non-negative integer. There cannot be two service stations have the same distance.

Each service station has a fleet of electric vehicles available for rental. Each vehicle has a range, which is the distance it can travel on a single charge, expressed in kilometers as a positive integer. A single service station can have up to 512 vehicles for rental.

When a driver rents a vehicle from a station 's', they can reach any station whose distance from 's' is less than or equal to the vehicle's range. A trip is defined by a sequence of service stations where the driver makes stops. It begins at one station and ends at another, passing through zero or more intermediate stations. The driver cannot backtrack during the trip, and a new vehicle must be rented at each station where they stop.

In summary, the key constraints are:

- The driver cannot return to a previously visited station.
- The next station ('t') must be farther from the starting point than the current station ('s').
- The next station ('t') must be reachable using one of the available vehicles at station 's'.

In cases where multiple routes have the same minimum number of stops, the system should chose the route which prioritizes the stations with shorter distances from the start of the highway.

In other words, we consider the set of routes with the same minimum number of stops as 'P = {p1, p2, ... pn}', where each route 'pi' is represented as a tuple of 'm' elements 'pi = ⟨pi,1, pi,2, ... pi,m⟩', which correspond to the distances from the start of the highway for each stop in order. The objective is to select the unique route 'pi' such that there is no other route 'pj' with the same 'k' stops preceded by a station with a shorter distance (i.e. ∄j, k : ⟨pi,m−k+1, . . . pi,m⟩ = ⟨pj,m−k+1, . . . pj,m⟩ ∧ pj,m−k < pi,m−k).

## Project Objective

The goal of this project is to plan a route between two service stations with the minimum number of stops in the most time and space -efficient  way. 

## Input Commands

The input text file contains a sequence of commands, one per line, with the following format. All positive or non-negative integer values can be represented as 32-bit integers.

### 'aggiungi-stazione distance number-of-cars autonomy-car-1 ... autonomy-car-n'

Adds a station at the specified distance, equipped with the given number of cars, each with its autonomy. Expected response: "aggiunta" or "non aggiunta".

### 'demolisci-stazione distance'

Removes the station at the specified distance, if it exists. Expected response: "demolita" or "non demolita".

### 'aggiungi-auto station-distance autonomy-car-to-add'

If the station exists, this command adds a car to it. Multiple cars with the same autonomy can be added. Expected response: "aggiunta" or "non aggiunta".

### 'rottama-auto station-distance autonomy-car-to-scrap'
Removes a car from the indicated station, if the station exists and has at least one car with the specified autonomy. Expected response: "rottamata" or "non rottamata".

### 'pianifica-percorso starting-station-distance destination-station-distance'
Requests the planning of a route with the constraints mentioned above. Expected response: the stops in order of travel, represented by the distance of the stations from the start of the highway, separated by spaces and followed by a newline at the end. The starting and destination stations must be included; if they coincide, the station is printed only once. If the route does not exist, it prints "nessun percorso". The planning action does not alter the stations or their vehicle fleets. The given stations will still be present after this command.


