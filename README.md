#Find Path

## Problem Description

Consider a highway, which can be represented as a sequence of service stations. Each service station is uniquely identified by its distance from the starting point of the highway, expressed in kilometers as a non-negative integer. No two service stations have the same distance.

Each service station has a fleet of electric vehicles available for rental. Each vehicle has a range, which is the distance it can travel on a single charge, expressed in kilometers as a positive integer. A single service station can have up to 512 vehicles for rental.

When a driver rents a vehicle from a station 's', they can reach any station whose distance from 's' is less than or equal to the vehicle's range. A trip is defined by a sequence of service stations where the driver makes stops. It begins at one station and ends at another, passing through zero or more intermediate stations. The driver cannot backtrack during the trip, and a new vehicle must be rented at each station where they stop.

In summary, the key constraints are:

- The driver cannot return to a previously visited station.
- The next station ('t') must be farther from the starting point than the current station ('s').
- The next station ('t') must be reachable using one of the available vehicles at station 's'.

## Project Objective

The goal of this project is to plan a route between two service stations with the minimum number of stops. In cases where multiple routes have the same minimum number of stops, the system should prioritize routes with shorter distances from the start of the highway.

In other words, we consider the set of routes with the same minimum number of stops as 'P = {p1, p2, ... pn}', where each route 'pi' is represented as a tuple of 'm' elements 'pi = ⟨pi,1, pi,2, ... pi,m⟩', which correspond to the distances from the start of the highway for each stop in order. The objective is to select the unique route 'pi' such that there is no other route 'pj' with the same 'k' final stops preceded by a station with a shorter distance.
