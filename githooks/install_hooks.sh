current_dir=$(pwd);

case "$current_dir" in 
    *githooks*)
        FILES=$(find -type f -not -path "*.sh" -not -path "*.bat")

        for FILE in $FILES; do
            echo "linking $FILE";
            ln -s $FILE ../.git/hooks/pre-commit;
        done

        return 0;
        ;;
esac

echo "this script must be run in githooks folder!";
return 1;
