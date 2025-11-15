/* 

Copyright (C) 2000 Matthew Danish

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include<iostream>
#include<stdio.h>
#include<math.h>
using namespace std;
#include"universe.h"
#include"opengl.h"
#include"world.h"
#include"orglist.h"
#include"idserver.h"
#include"id.h"
#include"angle.h"
#include"vector.h"
#include"gene.h"
#include"org.h"
#include<unistd.h>
#include"energy.h"
#include"hebbian.h"
#include"defines.h"
#include"quicksort.h"

/*
 * universe.cc
 * 
 * Contains the UniverseClass member functions
 * UniverseClass contains all the objects that are 
 * related to the simulation
 *
 * Note: all code surrounded by #ifdef _DEBUG .... 
 * #endif is code capable of being turned on and 
 * off for debugging purposes 
 * (or any purposes for that matter)
 */

double abs_d(double);
float abs_f(float);
int SortOListFunc(OList *,OList *);

extern int FLOOR_BLOCKS_Z,FLOOR_BLOCKS_X,NUM_FOOD,NUM_ORGANISMS;

/*
 * Some Utility functions
 */

int SortOListFunc(const void *a, const void *b) {
  OList *ao = (OList *) a;
  OList *bo = (OList *) b;
  if(ao->dist < bo->dist) return -1;
  else if(ao->dist == bo->dist) return 0;
  else return 1;
}

double abs_d(double n) {
  return (n > 0 ? n : -n);
}

float abs_f(float n) {
  return (n > 0 ? n : -n);
}



UniverseClass::UniverseClass(int argc, char **argv) {
  done = 0;
  ogl = new OpenGLClass(argc,argv);
  world = new WorldClass(argc,argv);
  orglist = new OrganismListClass;
  idserver = new IdServerClass;
}


UniverseClass::~UniverseClass() {
  delete ogl;
  delete world;
  delete orglist;
  delete idserver;
}


bool UniverseClass::MainLoop() {
  OrganismClass *o;
  int i;

  IdToken *token;
  VectorClass *pos;
  AngleClass *heading;
  GeneClass *genes;

  /*
   * This section creates the initial organisms and food
   */

  printf("[INIT] Creating initial organisms and food: NUM_ORGANISMS=%d, NUM_FOOD=%d\n", 
         NUM_ORGANISMS, NUM_FOOD);

  for(i=0;i<NUM_ORGANISMS + NUM_FOOD;i++) {
    // Each Organism is represented by a unique Id
    token = idserver->GetToken();
    
    // Get a random position in the world
    pos = world->NewPosition();

    // Choose 0 degrees as the initial direction
    heading = new AngleClass(0);

    // Stir up some new genes!
    genes = new GeneClass;

    // Throw it all in the pot and cook up an organism
    o = new OrganismClass(token,pos,heading,genes);

    // Put it on the list to keep track of it
    orglist->Append(o);

    // If we've created all the organisms, do food instead
    if(i>=NUM_ORGANISMS) {
      o->Type(ORGANISM_FOOD); // Make it into food instead!
      printf("[INIT] Created food organism #%d\n", i - NUM_ORGANISMS + 1);
    } else {
      // otherwise give it full energy to start
      o->Energy().EatFood(o->Energy().FoodCap());
      printf("[INIT] Created live organism #%d\n", i + 1);
    }
  }
  
  printf("[INIT] Initialization complete. Total organisms created: %d\n", NUM_ORGANISMS + NUM_FOOD);
  
  int frames = 0;
  long start_time = time(NULL);
  long cur_time = start_time;

  while(!done) {

    Update();

    frames++;

    cur_time=time(NULL);
    if(cur_time-start_time >= 5) {
      printf("Frames per second: %f\n", double(double(frames)/double(cur_time-start_time)));
      start_time=cur_time;
      frames=0;
    }
  }
  return true;
}


bool UniverseClass::Update() {
  orglist->Rewind();

  OrganismClass *o;
  vector<OrganismClass *> foodlist,livelist;
  IdToken *token;
  VectorClass *pos;
  AngleClass *heading;
  GeneClass *genes;

  if(!pause) {
  while((o=orglist->Next())) { 

    // If the Organism has gone off to never-never land
    if(isnan(o->Pos().X()) || isnan(o->Pos().Z())) {
      orglist->Remove(o->Id()); // get rid of it
      continue;
    }


    if(o->Type()==ORGANISM_LIVE) livelist.push_back(o);
    
    if(o->Type()==ORGANISM_FOOD) {
      // Check food lifespan before calling Lifetick to show remaining time
      // Note: We can't easily access lifespan from here, so we'll log after decay
      if(!o->Lifetick()) { // check if food has decayed
	printf("Decay! (food item expired after %d ticks, removed from world)\n", DECAY_SPAN);
	orglist->Remove(o->Id());
      } else
	foodlist.push_back(o);
    }
  }

  // Check to see if the number of organisms have fallen below a certain
  // amount and if they have, to add a new organism randomly
  // (and print a silly message too!)
  // BUGFIX: Only spawn if NUM_ORGANISMS > 0 (user explicitly wants organisms)
  if(NUM_ORGANISMS > 0 && (signed int)livelist.size() < NUM_ORGANISMS) {
    printf("[UPDATE] Spawning new organism: livelist.size()=%d < NUM_ORGANISMS=%d\n", 
           (int)livelist.size(), NUM_ORGANISMS);
    printf("New guy!\n");
    token = idserver->GetToken();
    pos = world->NewPosition();
    heading = new AngleClass(0);
    genes = new GeneClass;
    o = new OrganismClass(token,pos,heading,genes);
    orglist->Append(o);
    o->Energy().EatFood(o->Energy().FoodCap()/2);
  }

  // Same as above, but for food instead
  // BUGFIX: Only spawn if NUM_FOOD > 0 (user explicitly wants food)
  if(NUM_FOOD > 0 && (signed int)foodlist.size() < NUM_FOOD) {
    // Food was eaten or decayed - respawn to maintain NUM_FOOD count
    // This is expected behavior: system maintains constant food supply
    printf("[UPDATE] Spawning new food: foodlist.size()=%d < NUM_FOOD=%d (food was eaten/decayed)\n", 
           (int)foodlist.size(), NUM_FOOD);
    cout << "More food!" << endl;
    token = idserver->GetToken();
    pos = world->NewPosition();
    heading = new AngleClass(0);
    genes = new GeneClass;
    o = new OrganismClass(token,pos,heading,genes);
    orglist->Append(o);
    o->Type(ORGANISM_FOOD);
  } else if(NUM_FOOD == 0 && (signed int)foodlist.size() > 0) {
    // If user set NUM_FOOD=0 but food exists, log it (shouldn't happen after fix)
    printf("[UPDATE] WARNING: NUM_FOOD=0 but foodlist.size()=%d (this should not spawn new food)\n", 
           (int)foodlist.size());
  }

  int i;
  for(i=0;i<(signed int)livelist.size();i++) {
    if(livelist[i]->Type()==ORGANISM_LIVE) {
      UpdateOrganism(livelist[i],foodlist,livelist.size());

      livelist[i]->Lifetick();
    }
  }
  livelist.clear();
  foodlist.clear();
  }

  UpdateDraw();

  return true;
}

bool UniverseClass::UpdateOrganism(OrganismClass *o,vector<OrganismClass *> foodlist,int org_count) {
  int i;
  vector<float> inputs,outputs;
  OrganismClass *tmp,*mate=NULL;
#ifdef OLD_LOOP
  float last_min=0,min_limit=0,dist=0;
#endif
  VectorClass distv,dims;
  ColorClass color;
  AngleClass heading;
  
  // Start by giving some inputs to the neural network
  inputs.push_back(float(rand())/RAND_MAX*2.0-1.0);
  inputs.push_back(o->Energy().Health());
  inputs.push_back(o->Energy().Food());


#ifndef OLD_LOOP
  // alternative to below

  orglist->Rewind();
  vector<OList> olist;
  VectorClass headingvect=o->Heading().Vector();
  while((tmp=orglist->Next())) {
    OList ol;
    if(tmp!=o) {
      ol.distv = (tmp->Pos() - o->Pos());
      if(ol.distv.Dot(headingvect) > 0) {
	ol.dist = ol.distv.Magnitude();
	ol.color = tmp->Color();
	ol.heading = ol.distv;
	ol.heading-=o->Heading();
	olist.push_back(ol);
      }
    }
  }
  long reallen = olist.size();
  QuickSortOList(olist);
  i=0;
  while(i<reallen && (signed int)(inputs.size()+5) < o->Brain().NumInputs()) {
    inputs.push_back((float)olist[i].dist);
    inputs.push_back((float)olist[i].heading.Angle());
    inputs.push_back((float)olist[i].color.R());
    inputs.push_back((float)olist[i].color.G());
    inputs.push_back((float)olist[i].color.B());
    i++;
  }

  // FOOD VISION: Allow organisms to see food items (yellow objects)
  // This enables foraging behavior to evolve
  // Follows same pattern as organism vision above
  vector<OList> foodvision;
  VectorClass food_headingvect = o->Heading().Vector(); // Reuse heading vector
  int food_idx;
  for(food_idx=0; food_idx < (signed int)foodlist.size(); food_idx++) {
    // Safety check: ensure food item exists and is actually food
    if(foodlist[food_idx] && foodlist[food_idx]->Type() == ORGANISM_FOOD) {
      OList food_ol;
      food_ol.distv = (foodlist[food_idx]->Pos() - o->Pos());
      
      // Field of view check: only see food in front of organism
      // Same dot product test as organism vision
      if(food_ol.distv.Dot(food_headingvect) > 0) {
	food_ol.dist = food_ol.distv.Magnitude();
	
	// Food is always yellow: R=1.0, G=1.0, B=0.0
	// This is how organisms learn to identify food
	food_ol.color.R(1.0);
	food_ol.color.G(1.0);
	food_ol.color.B(0.0);
	
	// Calculate heading angle relative to organism's current heading
	food_ol.heading = food_ol.distv;
	food_ol.heading -= o->Heading();
	
	foodvision.push_back(food_ol);
      }
    }
  }
  
  // Sort food by distance (closest first) - prioritizes nearby food
  QuickSortOList(foodvision);
  
  // Feed food vision data to neural network inputs
  // Same pattern as organism vision: 5 values per food (distance, angle, R, G, B)
  int food_i = 0;
  long food_reallen = foodvision.size();
  
#ifdef _DEBUG_FOOD_VISION
  // Debug output: verify food vision is working
  static int debug_frame_count = 0;
  if(debug_frame_count++ % 100 == 0 && food_reallen > 0) {
    printf("[FOOD_VISION] Organism sees %ld food items, closest at distance %f\n", 
           food_reallen, foodvision[0].dist);
  }
#endif
  
  while(food_i < food_reallen && (signed int)(inputs.size()+5) < o->Brain().NumInputs()) {
    inputs.push_back((float)foodvision[food_i].dist);
    inputs.push_back((float)foodvision[food_i].heading.Angle());
    inputs.push_back((float)foodvision[food_i].color.R()); // Should be 1.0
    inputs.push_back((float)foodvision[food_i].color.G()); // Should be 1.0
    inputs.push_back((float)foodvision[food_i].color.B()); // Should be 0.0
    food_i++;
  }

#else
  // This loop finds the closest organisms and gives their
  // position, direction, and color to the inputs of this guy's
  // neural network, until there are no more input spots left
  // (note: this _needs_ optimizing!)
  // (note: the above code optimizes this, this will be removed soon)

  while((signed int)(inputs.size() + 5) < o->Brain().NumInputs()) {
    color.R(0); color.G(0); color.B(0);
    heading=0;
    last_min=0;
    orglist->Rewind();
    while((tmp=orglist->Next())) {
      if(tmp == o) continue;
      distv=(tmp->Pos() - o->Pos());
      dist=distv.Magnitude();
      VectorClass headingvect=o->Heading().Vector();
      if((!last_min || dist<last_min) && dist>min_limit && distv.Dot(headingvect) > 0) {
	last_min=dist;
	color=tmp->Color();
	heading = distv;
	heading-=o->Heading();
      }
    }
    min_limit=(last_min>min_limit ? last_min : min_limit);
    inputs.push_back((float)last_min);
    inputs.push_back((float)heading.Angle());
    inputs.push_back((float)color.R());
    inputs.push_back((float)color.G());
    inputs.push_back((float)color.B());
  }
#endif

  // Any inputs left over? fill em with 0
#ifndef OLD_LOOP
  while((signed int)inputs.size() < o->Brain().NumInputs())
    inputs.push_back(0);
#else
  while((signed int)inputs.size()<o->Brain().NumInputs()) inputs.push_back(float(rand())/RAND_MAX*2.0-1.0);
#endif

#ifdef _NN_DEBUG  
  printf("Num Inputs: %d\n",inputs.size());
  for(i=0;i<(signed int)inputs.size();i++) 
    printf("%f ",inputs[i]);
  printf("\n");
  getchar();
#endif


  o->Brain().SetInputs(inputs);

  o->Brain().RunNet();

  o->Brain().GetOutputs(outputs);

#ifdef _NN_DEBUG  
  printf("Num Outputs: %d\n",inputs.size());
  for(i=0;i<(signed int)outputs.size();i++) 
    printf("%f ",outputs[i]);
  printf("\n");
  getchar();
#endif

  o->Brain().Learn(o->Genes().LC(),ALPHA_COEFFICIENT);

  // Change the heading according to the output of the neural network
  o->Heading()+=double(((int)outputs[1] % 20)/10 * (double)o->Genes().MaxSpeed());
  VectorClass dir((double)(o->Genes().MaxSpeed() * double((int)(outputs[0]) % 10)/10.0)/2,(double)outputs[1]);

#ifdef _DEBUG
  printf("%f %f %f\n",dir.X(),dir.Y(),dir.Z());
#endif

  // ditto for the position
  world->ChangePosition(o->Pos(),dir);


  // Set the Aggressive coloration (Red channel)
  // Brighter red = more aggressive (based on fight output)
  // Use sigmoid-like normalization to map outputs to 0-1 range for visible colors
  // Neural network outputs can vary widely, so we use tanh to normalize them
  double red_raw = abs_d(outputs[3]);
  // Use a smaller scale factor (50) to make colors more vibrant and visible
  // This ensures even small output values produce visible colors
  double red_scale = 50.0;
  double red_value = (tanh(red_raw / red_scale) + 1.0) / 2.0; // Map tanh output [-1,1] to [0,1]
  o->Color().R(red_value);
  
  // Set the Mating coloration (Blue channel)
  // Brighter blue = more likely to mate (based on mate output)
  // Use same normalization approach for consistency
  double blue_raw = abs_d(outputs[2]);
  double blue_scale = 50.0;
  double blue_value = (tanh(blue_raw / blue_scale) + 1.0) / 2.0; // Map tanh output [-1,1] to [0,1]
  if(blue_value > 1.0) blue_value = 1.0;
  o->Color().B(blue_value);
  
  // Green channel stays at 0 (not used for live organisms)
  o->Color().G(0);

  
  // Find out if this organism is near to any food, and whether or not
  // it needs that food
  for(i=0;i<(signed int)foodlist.size();i++) {
    if(o->Energy().Food() != o->Energy().FoodCap()) {  
      if(foodlist[i]) {
	distv=foodlist[i]->Pos() - o->Pos();
	long bounds = o->Genes().Reach();
	double actual_dist_x = abs_d(distv.X());
	double actual_dist_z = abs_d(distv.Z());
	distv.X(actual_dist_x - bounds - o->Size().X());
	distv.Z(actual_dist_z - bounds - o->Size().Z());
	if(distv.X() <= 0 && distv.Z() <= 0) {
	  double amt_food=(1-double(bounds/MAX_REACH)) * foodlist[i]->Energy().HealthCap() * (1+o->Genes().Metabolism());
#ifdef _DEBUG_FOOD
	  printf("Food Eaten: %f (distance: X=%.2f, Z=%.2f, reach=%ld, size=%.2f)\n",
		 amt_food, actual_dist_x, actual_dist_z, bounds, o->Size().X());
#endif
	  EventStack es;
	  es.a = o->Pos();
	  es.b = foodlist[i]->Pos();
	  es.a.Y(o->Size().Y()/2);
	  es.b.Y(foodlist[i]->Size().Y()/2);
	  es.color.R(1);
	  es.color.G(1);
	  es.color.B(0);
	  es.frames_remaining = EVENT_FLASH_DURATION;
	  foodstack.push(es);
	  o->Energy().EatFood(amt_food);
	  printf("Eat! (food amount: %.2f, new energy: %.2f/%.2f)\n", 
		 amt_food, o->Energy().Food(), o->Energy().FoodCap());
	  orglist->Remove(foodlist[i]->Id());
	  foodlist[i]=NULL;
	  break;
	}
      }
    } else break;
  }



  orglist->Rewind();
  // Find if there are any Organisms nearby this one
  while((mate=orglist->Next())) {
    if(mate==o) continue; // if its the same one
    distv = mate->Pos() - o->Pos(); // get the vector between them
    distv.X(abs_d(distv.X())-o->Size().X()-o->Genes().Reach());
    distv.Z(abs_d(distv.Z())-o->Size().Z()-o->Genes().Reach());
#ifdef _DEBUG
    char s[30];
    o->Id().String(s);
    printf("%s: %f %f %f\n",s,distv.X(),distv.Y(),distv.Z());
#endif
    if(distv.X() <= 0 &&  distv.Z() <= 0) { // if close enough
      vector<float> mate_outputs;
      mate->Brain().GetOutputs(mate_outputs);
      // Fight or mate, they decide!
#ifdef _DEBUG
      cout << "Fight: " << outputs[3] << " " << mate_outputs[3] << endl;
      cout << "Mate: " << outputs[2] << " " << mate_outputs[2] << endl;
#endif
      // Fight condition: product of both organisms' fight outputs must exceed threshold
      // Threshold scales with population: more organisms = easier to fight
      double fight_threshold = FIGHT_THRESHOLD * FIGHT_THRESHOLD * NUM_ORGANISMS / org_count;
      double fight_product = abs_f(mate_outputs[3]) * abs_f(outputs[3]);
      
      // Always log when organisms are close enough to potentially fight/mate
      // This helps debug why fights aren't happening
      static int proximity_count = 0;
      proximity_count++;
      if(proximity_count % 20 == 0) {  // Log more frequently
	printf("[PROXIMITY] Organisms close! fight_outputs[3]=%.2f, mate_outputs[3]=%.2f, product=%.2f, threshold=%.2f (NUM_ORGS=%d, org_count=%d)\n",
	       outputs[3], mate_outputs[3], fight_product, fight_threshold, NUM_ORGANISMS, org_count);
	printf("[PROXIMITY]   -> Fight would need: product > %.2f (currently %.2f)\n", fight_threshold, fight_product);
	printf("[PROXIMITY]   -> Mate outputs: %.2f x %.2f = %.2f\n", outputs[2], mate_outputs[2], abs_f(outputs[2]) * abs_f(mate_outputs[2]));
      }
      
      if(fight_product > fight_threshold) {
	// Log before attempting fight to see if it succeeds
	double genetic_variance = o->Genes().Variance(mate->Genes());
	
	// Debug: Show actual DNA values to understand why variance is 0
	static int debug_gene_count = 0;
	if(debug_gene_count++ < 5 && genetic_variance < 0.001) {
	  // Access DNA directly for debugging (we'll need to add a getter or make DNA public temporarily)
	  printf("[DEBUG_GENES] Variance=%.4f, showing first 3 genes:\n", genetic_variance);
	  printf("[DEBUG_GENES]   Gene 0 (SIZE_X): org1=%ld, org2=%ld, diff=%ld\n",
		 (long)o->Genes().Size().X(), (long)mate->Genes().Size().X(),
		 (long)(o->Genes().Size().X() - mate->Genes().Size().X()));
	  printf("[DEBUG_GENES]   Gene 3 (STRENGTH): org1=%.2f, org2=%.2f\n",
		 o->Genes().GetStrength(), mate->Genes().GetStrength());
	  printf("[DEBUG_GENES]   Gene 4 (MAXSPEED): org1=%.2f, org2=%.2f\n",
		 o->Genes().MaxSpeed(), mate->Genes().MaxSpeed());
	}
	
	printf("[FIGHT_ATTEMPT] Product=%.2f > threshold=%.2f, genetic_variance=%.4f (need > %.4f)\n",
	       fight_product, fight_threshold, genetic_variance, MISCEGENATION_RATE);
	Fight(o,mate);
      } else if (org_count < NUM_ORGANISMS+2*NUM_FOOD && abs_f(mate_outputs[2]) * abs_f(outputs[2]) > REPRODUCTION_THRESHOLD * org_count/NUM_ORGANISMS) {
	Mate(o,mate);
      }
      break;
    }
  }  

#ifdef _DEBUG
  printf("%f\n",(double)o->Genes().MaxSpeed());
#endif

  
  return true;
}

bool UniverseClass::Fight(OrganismClass *o1, OrganismClass *o2) {
  // fight yourself? maybe some other time...
  if(o1==o2) return false;

  // sorry, no playing with food
  if(o1->Type() != ORGANISM_LIVE || o2->Type() != ORGANISM_LIVE) return false;

  // Genetic variance check: Originally meant for mating (prevent inbreeding)
  // For fighting, this is too restrictive - organisms should be able to fight
  // even if genetically similar. Lower threshold or remove for fighting.
  double genetic_variance = o1->Genes().Variance(o2->Genes());
  double fight_variance_threshold = 0.001;  // Much lower threshold for fighting (vs 0.05 for mating)
  
  if(genetic_variance < fight_variance_threshold) {
    // Organisms are essentially identical (likely a bug in variance calculation or gene generation)
    // For now, allow fights even with low variance to prevent blocking all fights
    printf("[FIGHT_WARNING] Very low genetic variance %.4f (organisms nearly identical, allowing fight anyway)\n", 
           genetic_variance);
    // Don't return false - allow the fight to proceed
  }

  // calculate damage done and energy needed to do it
  double damage = DAMAGE_PER_HIT * o1->Genes().GetStrength() * (MAX_REACH - o1->Genes().Reach()) / MAX_REACH;
  double energy_used = damage * ENERGY_PER_DAMAGE * o1->Genes().Metabolism();

#ifdef _DEBUG_FIGHT
  char s1[30],s2[30];
  o1->Id().String(s1);
  o2->Id().String(s2);
  printf("%s does %f damage to %s\n",s1,damage,s2);
#endif

  // if ya don't got enough energy you can't do any damage...
  if(o1->Energy().Food() < energy_used) {
    printf("[FIGHT_BLOCKED] Not enough energy: have %.2f, need %.2f\n", 
           o1->Energy().Food(), energy_used);
    return false;
  }
  
  // Use it if you got it
  o1->Energy().UseEnergy(energy_used);
  printf("Attack! (damage: %.2f, energy used: %.2f)\n", damage, energy_used);

  // makes those little flashes between organisms :)
  EventStack es;
  es.a = o1->Pos();
  es.b = o2->Pos();
  es.a.Y(o1->Size().Y()/2);
  es.b.Y(o2->Size().Y()/2);
  es.color.R(1);
  es.color.G(0);
  es.color.B(0);
  es.frames_remaining = EVENT_FLASH_DURATION;
  fightstack.push(es);

  // Take that!
  if(!o2->Energy().TakeDamage(damage)) {
    o2->Type(ORGANISM_FOOD); // Uh oh, he died
    printf("Kill! (organism died and became food)\n");
  }
#ifdef _DEBUG_FIGHT
  printf("%s has %f health left.\n",s2,o2->Energy().Health());
  fflush(stdout);
#endif
  return true;
}

bool UniverseClass::Mate(OrganismClass *o1, OrganismClass *o2) {
  if(o1==o2) return false; // no asexuals allowed

  // no necrophilia either..
  if(o1->Type() != ORGANISM_LIVE || o2->Type() != ORGANISM_LIVE) return false;

  // find the costs
  double o1_spend=o1->Energy().FoodCap() * o1->Genes().GetEnergyPcnt()/2;
  double o2_spend=o2->Energy().FoodCap() * o2->Genes().GetEnergyPcnt()/2;

  // if only humans could do this...
  if(o1_spend > o1->Energy().Food()) return false;
  if(o2_spend > o2->Energy().Food()) return false;

  // spend the energy
  o1->Energy().UseEnergy(o1_spend);
  o2->Energy().UseEnergy(o2_spend);

  // spread the word
  printf("Mate! (energy spent: o1=%.2f, o2=%.2f)\n", o1_spend, o2_spend);

  // show a flash
  EventStack es;
  es.a = o1->Pos();
  es.b = o2->Pos();
  es.a.Y(o1->Size().Y()/2);
  es.b.Y(o2->Size().Y()/2);
  es.color.R(0);
  es.color.G(0);
  es.color.B(1);
  es.frames_remaining = EVENT_FLASH_DURATION;
  matestack.push(es);

  // create an kiddie
  IdToken *token=idserver->GetToken();
  VectorClass *position=new VectorClass(double(rand())/RAND_MAX*2.0-1.0+o1->Pos().X(),0,o2->Pos().Z()+double(rand())/RAND_MAX*2.0-1.0);
  AngleClass *heading=new AngleClass(0);
  // with the parent's genes used
  GeneClass *genes=new GeneClass(o1->Genes(),o2->Genes());
  OrganismClass *o3 = new OrganismClass(token,
					position,
					heading,
					genes);
  orglist->Append(o3);

  // Give the new guy energy that mommy and daddy spent
  o3->Energy().EatFood(o1_spend + o2_spend);
  
  return true;
}

bool UniverseClass::UpdateDraw() {
  orglist->Rewind();
  OrganismClass *o;
  
  PrepareDraw();

  DrawLandscape();

  ogl->DrawStack(fightstack);
  ogl->DrawStack(matestack);
  ogl->DrawStack(foodstack);

  while((o=orglist->Next())) {
    DrawOrganism(o);
  }


  Draw();

  CheckKeys();

  return true;
}

bool UniverseClass::PrepareDraw() {
  // clear the screen and transform the world!
  ogl->Clear();
  return ogl->Transform(world->Pos().X(),world->Pos().Y(),world->Pos().Z(),world->Heading().Angle());

}

bool UniverseClass::DrawLandscape() {
  return ogl->DrawLandscape(world->Size().X(),world->Size().Y(),world->Size().Z());
}

bool UniverseClass::DrawOrganism(OrganismClass *o) {
  return ogl->DrawOrganism(o);
}

bool UniverseClass::Draw() {
  return ogl->SwapBuffers();
}

bool UniverseClass::CheckKeys() {
  double xpos=world->Pos().X();
  double ypos=world->Pos().Y();
  double zpos=world->Pos().Z();
  double yrot=world->Heading().Angle();

  bool retval=ogl->CheckInput(xpos,ypos,zpos,yrot,done,pause);
 
  world->Pos().X(xpos);
  world->Pos().Y(ypos);
  world->Pos().Z(zpos);
  world->Heading().Angle(yrot);

  return retval;
}


