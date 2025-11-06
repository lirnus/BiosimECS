# Biosim - ECS
by Linus Müller 2025<br>
This repo is a continuation of the Evolution-Simulieren project by Moritz Wagner and me.
Just as its predecessor, this project aims to observe evolution and adaptation of randomly generated populations under certain selective stresses.<br>
One of the limiting factors of the older version, which was written completely in Python, was its poor performance and thus scalability. The goal of this newer version is to implement all (or most) of the previous features while using more computationally efficient code. To do this, I switched programming languages from Python to C++ to gain more control over low-level allocation schedules and grant the compiler more opportunities for optimization.<br>
There is also a focus on readability of the code and interactivity for users (for example, simulation parameters should be changeable through an extern textfile, not inside the source code). The simulation also aims to be as portable as possible, requiring no external libraries (except for the rendering done in Python, which is optional).<br>
Last but not least, new features (like new neurons, environment settings, selection criteria, ...) should also be able to be added at any time in the future, which is why there is a heavy focus on modularity and cross-compability of all systems applied in the simulation.<br>

## The simulation
The principles of this simulation approach are adapted from David Miller's video [I programmed some creatures. They Evolved](https://www.youtube.com/watch?v=N3tRFayqVtk&t=1637s), to which the visualizations are also quite similar.<br>
### How it works
The basic principle of the Biosim is quite simple to understand and stems directly from natural biological systems: We simulate a population of creatures (*Pixies*) on a two-dimensional grid. These Pixies exhibit different behaviours based on their own *genome*, which gets randomly generated at the start of each simulation.<br>
The behaviour consists of simple patterns, which have the potential to become more and more complex the more genes/neurons are enabled. The basic structure of a Pixie "brain" consists of single "*neurons*" that execute certain functions and return values to activate other neurons or execute actions themselves. There are sensory neurons that can interpret the surroundings, like neighbourhood density or relative position in the world, which lead to action neurons that can for example exert a movement in a certain direction.<br>
The part that gets encoded by the individual pixie genomes are the connections between these neurons - each gene is a 32 bit integer coding for the source, sink and weight of a single connection. These 32bit ints get generated randomly at the start of a generation. The resulting behaviour is also random and could look like this:
<p align="center">
    <img src=https://github.com/user-attachments/assets/fcf25bee-b827-44f2-9052-5a8dad314d96>
    <br>
    <em>A population of randomly generated pixies</em>
</p>

In the next step, a selection pressure gets applied on these pixies, causing some to perish and others to reproduce, passing on their genes to the next generation.<br>
This selection pressure could for example be to only select Pixies on the left half of the grid. Over time, one would expect to mostly find Pixies that exhibit a behaviour that more frequently leads to them being on the left half, and indeed that is what we find:<br>
<p align="center">
    <img src="https://github.com/user-attachments/assets/44ffe40f-d082-4010-bf29-fe369d3baf54">
    <br>
    <em>After only 20 generations, an adaptation response to the selection criterion "kill right half" has been found.</em>
</p>

### Effects to be studied
The simulation realizes or has the potential to realize concepts like mutation, gene drift, adaptation, migration, advantages of sexual reproduction

### Experimental and Analytical features
The Biosim has multiple features that enable a highly controlled experimental setup, detailed analyis and maximum reproducibility.<br>
#### Analysis features:
- The *metagenome* of each generation can be saved in a textfile, which allows for analysis on a population genetics level (see *Gene Frequencies*, *Mullerplot*) and visualization of Pixie brains
- The positions and movements of Pixies can be saved to render GIFs (as shown above)
- The survival rate or rather the mean fitness and the approximate diversity of a population can be saved and rendered
<p align="center">
    <img width="640" height="480" alt="survival_diversity_plot" src="https://github.com/user-attachments/assets/18bc0b9f-8bc9-4e0d-920a-91da0b633751" />
    <br>
    <em>Survival rates and diversity across generations</em>
</p>
<p align="center">
    <img width="640" height="480" alt="mullerplot1" src="https://github.com/user-attachments/assets/b9a7d91b-e3cc-49c7-8e37-94c32c561e40" />
    <br>
    <em>A "Mullerplot" of one simulation run, showing which strands are successful at which times</em>
</p>

<p align="center">
    <img width="1128" height="415" alt="pixiebrain2" src="https://github.com/user-attachments/assets/75125828-4705-4676-9f91-709007b38d13" />
    <br>
    <em>Visualization of a single Pixie brain (only neurons with connections are plotted)</em>
</p>

#### Setup features:
- A random seed can be set at the start of the simulation to guarantee reproducible results
- The *metagenome* of a previously simulated generation can be fed back into the program as a starting population (through this, you can simulate clonal populations of "established"(=already adapted) Pixies)
- single Neurons can be manually dis- or enabled to reduce or increase complexity in behaviour
- different environments (grids with obstacles, dispersion of food) can be chosen by a key in the simconfig textfile
