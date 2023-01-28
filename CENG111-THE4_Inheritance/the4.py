class Person:
    def __init__(self,Name,Mother="",Father="",Spouse="",Children=[],Status="Alive"):
        self.Name=Name
        self.Mother=Mother
        self.Father=Father
        self.Spouse=Spouse
        self.Children=[]
        self.Children[:]=Children
        self.Status=Status
    def GetName(self):
        return self.Name
    def SetMother(self,Mother):
        self.Mother = Mother
    def GetMother(self):
        return self.Mother
    def SetFather(self,Father):
        self.Father = Father
    def GetFather(self):
        return self.Father
    def SetSpouse(self,Spouse):
        self.Spouse = Spouse
    def GetSpouse(self):
        return self.Spouse
    def SetChildren(self,Children):
        self.Children += Children
    def GetChildren(self):
        return self.Children
    def SetStatus(self,Status):
        self.Status = Status
    def GetStatus(self):
        return self.Status
    def PrintPerson(self):
        print("Name:"+self.Name, "Mother:"+self.Mother, "Father:"+self.Father, "Spouse:"+self.Spouse, "Children:"+str(self.Children), "Status:"+self.Status)
    def RemoveDescendant(self,Descendant):
        self.Children.remove(Descendant)



def inheritance(Descriptions):
    Descriptions[:] = sorted(Descriptions)
    People = []
    for DescriptionSTR in Descriptions:
        Description = DescriptionSTR.split()
        if Description[0] == "CHILD":
            if Description[1] not in People:
                People += [Description[1]]
                globals()[Description[1]] = Person(Description[1],"","","",Description[3:])
            elif Description[1] in People:
                globals()[Description[1]].SetChildren(Description[3:])
            else:
                return "Error: Inheritance > People > CHILD > Mother"
            if Description[2] not in People:
                People += [Description[2]]
                globals()[Description[2]] = Person(Description[2],"","","",Description[3:])
            elif Description[2] in People:
                globals()[Description[2]].SetChildren(Description[3:])
            else:
                return "Error: Inheritance > People > CHILD > Father"
            for Child in Description[3:]:
                if Child not in People:
                    People += [Child]
                    globals()[Child] = Person(Child,Description[1],Description[2])
                elif Child in People:
                    globals()[Child].SetMother(Description[1])
                    globals()[Child].SetFather(Description[2])
                else:
                    return "Error: Inheritance > People > CHILD > Children"
        if Description[0] == "DECEASED":
            DeceasedName = Description[1]
            Inheritance = float(Description[2])
        if Description[0] == "DEPARTED":
            if Description[1] not in People:
                People += [Description[1]]
                globals()[Description[1]] = Person(Description[1],"","","",[],"Departed")
            elif Description[1] in People:
                globals()[Description[1]].SetStatus("Departed")
        if Description[0] == "MARRIED":
            if Description[1] not in People:
                People += Description[1]
                globals()[Description[1]] = Person(Description[1],"","",Description[2])
            elif Description[1] in People:
                globals()[Description[1]].SetSpouse(Description[2])
            if Description[2] not in People:
                People += Description[2]
                globals()[Description[2]] = Person(Description[2],"","",Description[1])
            elif Description[2] in People:
                globals()[Description[2]].SetSpouse(Description[1])
    Spouse = globals()[DeceasedName].GetSpouse()
    Heirs = FindHeirs(DeceasedName)
    if Heirs != [] and Spouse != "" and globals()[Spouse].GetStatus() == "Alive":
        return [(Spouse, (0.25)*Inheritance)] + DistributeInheritance(Heirs, (0.75)*Inheritance)
    elif Heirs != []:
        return DistributeInheritance(Heirs,Inheritance)
    elif Heirs == []:
        Mother = globals()[DeceasedName].GetMother()
        Father = globals()[DeceasedName].GetFather()
        if Mother == "" and Father == "":
            if Spouse != "" and globals()[Spouse].GetStatus() == "Alive":
                return [(Spouse,Inheritance)]
            else:
                return []
        globals()[Mother].RemoveDescendant(DeceasedName)
        globals()[Father].RemoveDescendant(DeceasedName)
        Heirs = FindHeirs("",[Mother,Father])
        if Heirs != [] and Spouse != "" and globals()[Spouse].GetStatus() == "Alive":
            return [(Spouse, (0.50)*Inheritance)] + DistributeInheritance(Heirs, (0.50)*Inheritance)
        elif Heirs != []:
            return DistributeInheritance(Heirs,Inheritance)
        elif Heirs == []:
            MMother = globals()[Mother].GetMother()
            MFather = globals()[Mother].GetFather()
            FMother = globals()[Father].GetMother()
            FFather = globals()[Father].GetFather()
            if MMother == "" and MFather == "" and FMother != "" and FFather != "":
                globals()[FMother].RemoveDescendant(Father)
                globals()[FFather].RemoveDescendant(Father)
                Heirs = FindHeirs("",[FMother,FFather])
                if Heirs != [] and Spouse != "" and globals()[Spouse].GetStatus() == "Alive":
                    return [(Spouse, (0.75)*Inheritance)] + DistributeInheritance(Heirs, (0.25)*Inheritance)
                elif Heirs != []:
                    return DistributeInheritance(Heirs,Inheritance)
                elif Spouse != "" and globals()[Spouse].GetStatus() == "Alive":
                    return [(Spouse,Inheritance)]
                else:
                    return []
            if MMother != "" and MFather != "" and FMother == "" and FFather == "":
                globals()[MMother].RemoveDescendant(Mother)
                globals()[MFather].RemoveDescendant(Mother)
                Heirs = FindHeirs("",[MMother,MFather])
                if Heirs != [] and Spouse != "" and globals()[Spouse].GetStatus() == "Alive":
                    return [(Spouse, (0.75)*Inheritance)] + DistributeInheritance(Heirs, (0.25)*Inheritance)
                elif Heirs != []:
                    return DistributeInheritance(Heirs,Inheritance)
                elif Spouse != "" and globals()[Spouse].GetStatus() == "Alive":
                    return [(Spouse,Inheritance)]
                else:
                    return []
            if MMother == "" and MFather == "" and FMother == "" and FFather == "":
                if Spouse != "" and globals()[Spouse].GetStatus() == "Alive":
                    return [(Spouse,Inheritance)]
                else:
                    return []
            globals()[MMother].RemoveDescendant(Mother)
            globals()[MFather].RemoveDescendant(Mother)
            globals()[FMother].RemoveDescendant(Father)
            globals()[FFather].RemoveDescendant(Father)
            Heirs = FindHeirs("",[MMother,MFather,FMother,FFather])
            if Heirs != [] and Spouse != "" and globals()[Spouse].GetStatus() == "Alive":
                return [(Spouse, (0.75)*Inheritance)] + DistributeInheritance(Heirs, (0.25)*Inheritance)
            elif Heirs != []:
                return DistributeInheritance(Heirs,Inheritance)
            elif Spouse != "" and globals()[Spouse].GetStatus() == "Alive":
                return [(Spouse,Inheritance)]
            else:
                return []

def FindHeirs(DeadName,PotentialHeirs=[]):
    if PotentialHeirs == []:
        PotentialHeirs = globals()[DeadName].GetChildren()
    Heirs = []
    for Member in PotentialHeirs:
        if globals()[Member].GetStatus() == "Alive":
            Heirs += [Member]
        elif globals()[Member].GetStatus() == "Departed" and FindHeirs(Member) != []:
            Heirs += [Member]
    return Heirs

def DistributeInheritance(Heirs,Inheritance):
    IndividualShare = Inheritance/len(Heirs)
    Result = []
    for Heir in Heirs:
        if globals()[Heir].GetStatus() == "Departed":
            Result += DistributeInheritance(FindHeirs(Heir),IndividualShare)
        elif globals()[Heir].GetStatus() == "Alive":
            Result += [(Heir,IndividualShare)]
    return Result