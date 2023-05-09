@0xedbee4f063ac76f9;

struct Person
{
    # A person credited in a comic issue.
    name @0 : Text;
}

struct Comic
{
    title @0 : Text;
    # Title of the comic series.
    issue @1 : Int16;
    # Issue number within the series.
    script @2 : Person;
    # Script credit
    pencils @3 : Person;
    # Pencils credit
    inks @4 : Person;
    # Inks credit
    colors @5 : Person;
    # Colors credit
    letters @6 : Person;
    # Lettering credit
}

interface Database
{
    list @0 () -> (list : List(Int64));         # List all known ids in the database
    create @2 (comic : Comic) -> (id : Int64);  # Create a comic, returns the id
    read @1 (id : Int64) -> (comic : Comic);    # Read a comic from the database by id
    update @3 (id : Int64, comic : Comic);      # Update a comic by id
    delete @4 (id : Int64);                     # Delete a comic by id
}
