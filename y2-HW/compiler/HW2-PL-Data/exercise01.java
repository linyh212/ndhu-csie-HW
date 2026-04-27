import java.util.*;

public class exercise01 {
    static class Person {
        String name, gender;
        Person spouse = null;
        List<Person> children = new ArrayList<>();
        List<Person> parents = new ArrayList<>();

        public Person(String name, String gender) {
            this.name = name;
            this.gender = gender;
        }

        public void addChild(Person child) {
            if (child == null || child == this) return;
            if (!children.contains(child)) {
                children.add(child);
            }
            if (!child.parents.contains(this)) {
                child.parents.add(this);
            }
            if (spouse != null) {
                if (!spouse.children.contains(child)) {
                    spouse.children.add(child);
                }
                if (!child.parents.contains(spouse)) {
                    child.parents.add(spouse);
                }
            }
        }
    }

    static Map<String, Person> people = new HashMap<>();

    static Person getPerson(String name) {
        return people.get(name);
    }

    static void createPerson(String name, String gender) {
        people.put(name, new Person(name, gender));
    }

    static void marry(String name1, String name2) {
        Person p1 = getPerson(name1);
        Person p2 = getPerson(name2);
        if (p1 != null && p2 != null) {
            p1.spouse = p2;
            p2.spouse = p1;
        }
    }

    static void addChild(String parentName, String childName) {
        Person parent = getPerson(parentName);
        Person child = getPerson(childName);
        if (parent != null && child != null) {
            parent.addChild(child);
        }
    }

    static boolean areSiblings(Person p1, Person p2) {
        if (p1 == null || p2 == null || p1 == p2) return false;
        for (Person parent1 : p1.parents) {
            if (parent1 != null && p2.parents.contains(parent1)) {
                return true;
            }
        }
        return false;
    }

    static boolean areBrothers(Person p1, Person p2) {
        return areSiblings(p1, p2) && p1.gender.equals("male") && p2.gender.equals("male");
    }

    static boolean areSisters(Person p1, Person p2) {
        return areSiblings(p1, p2) && p1.gender.equals("female") && p2.gender.equals("female");
    }

    static boolean areCousins(Person p1, Person p2) {
        if (p1 == null || p2 == null || p1 == p2) return false;
        if (areSiblings(p1, p2)) return false;
        for (Person p1Parent : p1.parents) {
            for (Person p2Parent : p2.parents) {
                if (p1Parent != null && p2Parent != null && areSiblings(p1Parent, p2Parent)) {
                    return true;
                }
            }
        }
        return false;
    }

    static boolean areMarried(String name1, String name2) {
        Person p1 = getPerson(name1);
        Person p2 = getPerson(name2);
        return p1 != null && p2 != null && p1.spouse == p2;
    }

    static boolean queryRelationship(String name1, String name2, String relation) {
        Person p1 = getPerson(name1);
        Person p2 = getPerson(name2);
        if (p1 == null || p2 == null) return false;
        switch (relation.toLowerCase()) {
            case "sibling":
                return areSiblings(p1, p2);
            case "brother":
                return areBrothers(p1, p2);
            case "sister":
                return areSisters(p1, p2);
            case "cousin":
                return areCousins(p1, p2);
            case "married":
                return areMarried(name1, name2);
            default:
                return false;
        }
    }

    public static void main(String[] args) {
        String[] males = "Andy, Bob, Cecil, Dennis, Edward, Felix, Martin, Oscar, Quinn".split(", ");
        String[] females = "Gigi, Helen, Iris, Jane, Kate, Liz, Nancy, Pattie, Rebecca".split(", ");
        for (String name : males) createPerson(name, "male");
        for (String name : females) createPerson(name, "female");
        
        marry("Bob", "Helen");
        marry("Dennis", "Pattie");
        marry("Gigi", "Martin");
        
        addChild("Andy", "Bob");
        addChild("Bob", "Cecil");
        addChild("Cecil", "Dennis");
        addChild("Dennis", "Edward");
        addChild("Edward", "Felix");

        addChild("Gigi", "Helen");
        addChild("Helen", "Iris");
        addChild("Iris", "Jane");
        addChild("Jane", "Kate");
        addChild("Kate", "Liz");

        addChild("Martin", "Nancy");
        addChild("Nancy", "Oscar");
        addChild("Oscar", "Pattie");
        addChild("Pattie", "Quinn");
        addChild("Quinn", "Rebecca");
        
        Scanner sc = new Scanner(System.in);
        System.out.println("Input format: Name1, Name2, Relationship");
        while (true) {
          String line = sc.nextLine().trim();
            if (line.equalsIgnoreCase("exit")) break;
            String[] parts = line.split(",");
            if (parts.length != 3) {
                System.out.println("Invalid input. Use format: Name1, Name2, Relationship");
                continue;
            }
            String name1 = parts[0].trim();
            String name2 = parts[1].trim();
            String relation = parts[2].trim();
            boolean result = queryRelationship(name1, name2, relation);
            System.out.println(result);
        }
        sc.close();
    }
}
