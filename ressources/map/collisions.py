import csv

def read_size_objects(filename):
    objects = {}
    with open(filename, 'r') as file:
        lines = file.readlines()
        nb_objects = int(lines[1].strip())  # Nombre d'objets à partir de la deuxième ligne
        i = 1
        for line in lines[2:2+nb_objects]:  # Lire les dimensions à partir de la troisième ligne
            parts = line.strip().split()
            if len(parts) == 2:
                width = int(parts[0])
                height = int(parts[1])
                objects[i] = (width, height)
                i+=1
    return objects

def update_map_object_csv(objects, csv_filename):
    # Charger la carte objet depuis le fichier CSV
    map_object = []
    with open(csv_filename, 'r') as file:
        reader = csv.reader(file,delimiter=' ')
        next(reader) 
        next(reader)
        for row in reader:
            map_object.append([int(val) for val in row])
    nbobject = 0
    # Mettre à jour la carte objet en remplaçant les valeurs autour des objets par -1
    for i in range(len(map_object)):
        for j in range(len(map_object[i])):
            object_id = map_object[i][j]
            if object_id != 0 and object_id in objects:
                width, height = objects[object_id]
                for di in range(min(len(map_object) - i, height)):
                    for dj in range(min(len(map_object[i]) - j, width)):
                        if map_object[i + di][j + dj] == 0:
                            if object_id == 20 or object_id == 21 or object_id == 19: 
                                map_object[i + di][j + dj] = -2
                            else :
                                map_object[i + di][j + dj] = -1
                
            if object_id != 0 and object_id != -1 and object_id != -2:
                nbobject +=1
    print(nbobject)
    width = len(map_object[0])  # La largeur est le nombre de colonnes
    height = len(map_object)     # La hauteur est le nombre de lignes

    # Écrire la carte objet mise à jour dans le fichier CSV avec la largeur et la hauteur
    with open(csv_filename, 'w', newline='') as file:
        writer = csv.writer(file, delimiter=' ')
        writer.writerow([height])
        writer.writerow([width])    
        writer.writerows(map_object)

if __name__ == '__main__':
    size_objects_filename = 'size_objects.txt'
    map_object_csv_filename = 'map_object.csv'

    # Lire les dimensions des objets à partir du fichier size_objects.txt
    objects_dimensions = read_size_objects(size_objects_filename)

    # Mettre à jour la carte objet (map_object.csv) en conséquence
    update_map_object_csv(objects_dimensions, map_object_csv_filename)

    print(f"La carte {map_object_csv_filename} a été mise à jour avec les valeurs -1 autour des objets.")
