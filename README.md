# Biosim - ECS
by Linus Müller 2025<br>
This repo is a continuation of the Evolution-Simulieren project by Moritz Wagner and me.
Just as its predecessor, this project aims to observe evolution and adaptation of randomly generated populations under certain selective stresses.<br>
One of the limiting factors of the older version, which was written completely in Python, was its poor performance and thus scalability. The goal of this newer version is to implement all (or most) of the previous features while using more computationally efficient code. To do this, I switched programming languages from Python to C++ to gain more control over low-level allocation schedules and grant the compiler more opportunities for optimization.<br>
There is also a focus on readability of the code and interactivity for users (for example, simulation parameters should be changeable through an extern textfile, not inside the source code).<br>
Last but not least, new features (like new neurons, environment settings, selection criteria, ...) should also be able to be added at any time in the future, which is why there is a heavy focus on modularity and cross-compability of all systems applied in the simulation.<br>

## The simulation
The principles of this simulation approach are adapted from David Miller's video [I programmed some creatures - they evolved](https://www.youtube.com/watch?v=N3tRFayqVtk&t=1637s), to which also the visualizations are quite similar.<br>
The basic principle of the Biosim is quite simple to understand and stems directly from natural biological systems: We simulate a population of creatures (*Pixies*) on a two-dimensional grid. These Pixies exhibit different behaviours based on their own *genome*, which gets randomly generated at the start of each simulation.


## Experimental and Analytical features
The Biosim has multiple features that enable a highly controlled experimental setup, detailed analyis and maximum reproducibility.<br>
Analysis features:
- The *metagenome* of each generation can be saved in a textfile, which allows for analysis on a population genetics level (see *Gene Frequencies*, *Mullerplot*) and visualization of Pixie brains
- The positions and movements of Pixies can be saved to render GIFs (as shown above)
Setup features:
- A random seed can be set at the start of the simulation to guarantee reproducible results
- The *metagenome* of a previously simulated generation can be fed back into the program as a starting population (through this, you can simulate clonal populations of "established"(=already adapted) Pixies)
- single Neurons can be manually dis- or enabled to reduce or increase complexity in behaviour
- different environments (grids with obstacles, dispersion of food) can be chosen by a key in the simconfig textfile
