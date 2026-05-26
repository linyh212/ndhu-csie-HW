datatype person = Person of {
    name : string,
    gender : string,
    spouse : person option ref,
    children : person list ref,
    parents : person list ref
}

val people = ref ([]: (string * person) list)

fun nameOf (Person r) = #name r
fun genderOf (Person r) = #gender r
fun spouseOf (Person r) = !(#spouse r)
fun childrenOf (Person r) = !(#children r)
fun parentsOf (Person r) = !(#parents r)

fun getPerson name =
    case List.find (fn (n, _) => n = name) (!people) of
        SOME (_, p) => SOME p
      | NONE => NONE

fun createPerson (name, gender) =
    let
        val newPerson = Person {
            name = name,
            gender = gender,
            spouse = ref NONE,
            children = ref [],
            parents = ref []
        }
    in
        people := (name, newPerson) :: (!people);
        newPerson
    end

fun addChild (parent, child) =
    let
        val pchildren = case parent of Person r => #children r
        val cparents = case child of Person r => #parents r
    in
        if nameOf parent = nameOf child then ()
        else (
            if not (List.exists (fn c => nameOf c = nameOf child) (!pchildren)) then
                pchildren := child :: (!pchildren)
            else ();
            if not (List.exists (fn p => nameOf p = nameOf parent) (!cparents)) then
                cparents := parent :: (!cparents)
            else ();
            case spouseOf parent of
                SOME s =>
                    let
                        val schildren = case s of Person r => #children r
                    in
                        if not (List.exists (fn c => nameOf c = nameOf child) (!schildren)) then
                            schildren := child :: (!schildren)
                        else ();
                        if not (List.exists (fn p => nameOf p = nameOf s) (!cparents)) then
                            cparents := s :: (!cparents)
                        else ()
                    end
              | NONE => ()
        )
    end

fun marry (name1, name2) =
    case (getPerson name1, getPerson name2) of
        (SOME (Person p1), SOME (Person p2)) =>
            (#spouse p1 := SOME (Person p2);
             #spouse p2 := SOME (Person p1))
      | _ => ()

fun addChildByName (parentName, childName) =
    case (getPerson parentName, getPerson childName) of
        (SOME p1, SOME p2) => addChild (p1, p2)
      | _ => ()

fun areSiblings (p1, p2) =
    if nameOf p1 = nameOf p2 then false
    else List.exists (fn parent1 =>
           List.exists (fn parent2 => nameOf parent1 = nameOf parent2) (parentsOf p2)
         ) (parentsOf p1)

fun areBrothers (p1, p2) = 
    areSiblings (p1, p2) andalso genderOf p1 = "male" andalso genderOf p2 = "male"

fun areSisters (p1, p2) = 
    areSiblings (p1, p2) andalso genderOf p1 = "female" andalso genderOf p2 = "female"

fun areCousins (p1, p2) =
    if nameOf p1 = nameOf p2 orelse areSiblings (p1, p2) then false
    else List.exists (fn parent1 =>
           List.exists (fn parent2 => areSiblings (parent1, parent2)) (parentsOf p2)
         ) (parentsOf p1)

fun areMarried (p1, p2) =
    case spouseOf p1 of
        SOME s => nameOf s = nameOf p2
      | NONE => false

fun queryRelationship (name1, name2, relation) =
    case (getPerson name1, getPerson name2) of
        (SOME p1, SOME p2) =>
            (case String.map Char.toLower relation of
                "sibling" => areSiblings (p1, p2)
              | "brother" => areBrothers (p1, p2)
              | "sister" => areSisters (p1, p2)
              | "cousin" => areCousins (p1, p2)
              | "married" => areMarried (p1, p2)
              | _ => false
            )
      | _ => false

val males = ["Andy", "Bob", "Cecil", "Dennis", "Edward", "Felix", "Martin", "Oscar", "Quinn"]
val females = ["Gigi", "Helen", "Iris", "Jane", "Kate", "Liz", "Nancy", "Pattie", "Rebecca"]

val _ = List.app (fn name => ignore (createPerson (name, "male"))) males
val _ = List.app (fn name => ignore (createPerson (name, "female"))) females

val _ = marry ("Bob", "Helen")
val _ = marry ("Dennis", "Pattie")
val _ = marry ("Gigi", "Martin")

val _ = addChildByName ("Andy", "Bob")
val _ = addChildByName ("Bob", "Cecil")
val _ = addChildByName ("Cecil", "Dennis")
val _ = addChildByName ("Dennis", "Edward")
val _ = addChildByName ("Edward", "Felix")
val _ = addChildByName ("Gigi", "Helen")
val _ = addChildByName ("Helen", "Iris")
val _ = addChildByName ("Iris", "Jane")
val _ = addChildByName ("Jane", "Kate")
val _ = addChildByName ("Kate", "Liz")
val _ = addChildByName ("Martin", "Nancy")
val _ = addChildByName ("Nancy", "Oscar")
val _ = addChildByName ("Oscar", "Pattie")
val _ = addChildByName ("Pattie", "Quinn")
val _ = addChildByName ("Quinn", "Rebecca")

fun boolToString true = "true"
  | boolToString false = "false"

fun main () =
    let
        val _ = print "Program started\n"
        val _ = print ("Are Felix and Liz cousins? " ^ boolToString (queryRelationship ("Felix", "Liz", "cousin")) ^ "\n")
        val _ = print ("Are Edward and Felix brothers? " ^ boolToString (queryRelationship ("Edward", "Felix", "brother")) ^ "\n")
        val _ = print ("Are Cecil and Iris sibling? " ^ boolToString (queryRelationship ("Cecil", "Iris", "sibling")) ^ "\n")
    in
        ()
    end

val _ = main ();
