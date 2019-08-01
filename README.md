# GoBang-AI
简单的五子棋人机对弈 （α—β裁剪与集成算杀）
算法之美
                ---基于五子棋AI的分析                                 
一：前言
但凡对编程有所了解的人应该都知道 程序=数据结构+算法，但是当一个编程小白第一次去接触抽象数据结构的时候，难免不会疑惑当你简单的数组遍历解决一个问题时，当看到大佬重新写一个那么长的链表时惊呆了，你10行冒泡排序解决的排序，当看到大佬们写的400+的快速排序时傻眼了，你费尽心思好不容易写出100多行的二叉查找树，当看到大佬们400+的红黑树时迷茫了，可能会问我这样不也是达到了目的吗？确实不能说你不对，在一定时间内你的程序当然可以解决问题，但是一秒钟解决是一种解决，一分钟解决是一种解决，一天，一月，或者说是到宇宙灭亡同样也是另一种解决，当有一天你看到数以亿几的数据在红黑树上仅花费一秒的时间就可以找到你要的那个结果时你的二叉查找树在一个小时后还在运行你可能会明白一点，而我想说的算法更是同数据结构一般，是一个程序的灵魂所在，当你看完我接下来的这篇基于五子棋AI的分析后，我相信你也一定会体会到数据结构，算法到底有有美

二：我对人工智能的看法
     在很久之前的AlphaGo 4:1击败李世石时世界哗然，宣告这人类在棋类中的最后一块不可战胜的领地被计算机攻破了，其实我开始时时的关注点是在那一场AlphaGo输的那一局，为什么会输，AlphaGo的设计上没用去研究任何一个人的确定性棋谱，而是全部依靠深度学习，自己和自己对弈或者和高手对弈获得技术上的提高，在AlphaGo输的那一晚，专业围棋手们都安心了，计算机不过如此，人类在围棋上的造诣是计算机不可超越的，而这一晚AlphaGo自己和自己又进行了数千万场的对弈，第二天李世石还是李世石，而AlphaGo早以不是昨天的那个AlphaGo，计算机与人类的局别在于计算机它的技术只会增加，不会下滑，更加不会发挥失常，在后面的对局中AlphaGo便是势如破竹，4:1战胜李世石，而它的全部学习时间只有四个月，从一个什么不会的新手到击败围棋九段的大师这不经让人惊叹人工智能的潜力。

三．五子棋AI设计
   在很久之前就想自己做一个小游戏，加之自己对人工智能一直比较感兴趣，就打算想做一个AI对弈的棋类游戏，一直没有什么时间，大一暑假的时候正好有些时间，就抽空出来写了这个程序，由于目前本人技术有限，不能像那些大牛一样写出那么强的AI，我这个程序的AI棋力大概就一般，但是绝对不弱（没对五子棋有深入研究的业余玩家一般是下不赢的）

程序关键部分讲解
1.	评估函数
像五子棋这样15-15的棋局大小，就算是计算机也没有办法从一开始就可以推算出所有的可能落子法，从而找到赢的方法，但是只找几步它还是可以做到的，比如假设一步有50种合适的走法，走四步（AI和人各走两步）也就是625万种走法，我们不考虑这么多走法要多少时间，关键的一点在于由于AI在四次深度的查找后不一定是找到赢的方法，那么计算机怎么知道这一次的选择是否合适，它不像人一样，可以通过棋局抽象的找到哪个棋子的位置合适落子，它需要的理性直观的东西，可以直接告诉它这么走有多好，而评估函数正是这样这个东西，它通过对整个棋盘的局势分析给出一个值，计算机凭借这个值的大小从而判断落子的好坏，一个评价函数的好坏也就直接决定这AI的棋力高低的因素之一，由于这个评价函数还涉及很多五子棋的棋型知识，都是100多行的if else之类的嵌套，和一些不入流的打分机制过于繁琐无趣，我就不详细讲解了，具体可以参考我后面的代码的sorce和wholeSorce函数部分

2.	极大极小值算法
从这里开始你会慢慢体会算法的美，我们乍一看这个算法的名字感觉好高深，其实不然，我抛开这个算法，讲一讲什么是博弈，比如你和一个好朋友以石头剪刀布的方法打赌10元钱，赢的人获得这10元，输的支付，这时你的好友说他一定会出石头，这时摆在你面前的一个问题就是你为了赢应该出什么，你相信他毫无疑问会出布，但是如果他骗你呢？他料定你会出布，这时他要赢会出剪刀，所以想到这里，你为了赢会出石头，但是如果他猜到你会猜到他在骗你，会出石头，他就会在这一次中出布，在这个问题中你所面临的问题就是一个你和他之间的博弈问题，我们为了赢会去猜测对方最好的对策，从而获得博弈中的胜利，由于没人想输，所以我在决定对策时会假设对方所做的选择是他的最好选择，而这个极大极小值算法就是这样一个考虑博弈中对策的算法，所以说你看不算法源于生活，在该算法中AI企图让自己的落子获得最大（极大）的评分，而玩家则想让AI的落子获得最小（极小）的评分，这样通过一个函数随机在可以落子的地方落子，企图让这个落子的评分最大，然后在通过一个对称的函数在可以落子的地方落子，企图让刚刚计算机的落子评分最低（这一步就是人类玩家最赞同的应招）,重复上面的两步就可以让计算机找到当前局势下的最优解了，具体查考后面的代码部分findCompMove和findHumanMove函数


3.	α-β裁剪
正如我在最开头提到过的一秒钟解决是一种解决，一分钟解决是一种解决，一天，一月，或者说是到宇宙灭亡同样也是另一种解决，随着我们查找深度的增加，需要查找的结点更是成指数增加，指数有多可怕，一步50结点，四步就达到了惊人的625万个结点，在我的i7的电脑中一秒大概可以查询50万个结点（i5的电脑性能只有i7的60%左右会更少），如果我们不采取措施，那电脑一步就要计算10多秒这样是无法接受的，但是还好我们有个解决办法，那就是裁剪，这棵博弈树太大了，但是其中有一些枝丫我们是不需要的，我们可以将其裁剪掉，在理论上（α-β裁剪非常依赖结点的访问顺序，必要时还要进行排序）通过α-β裁剪我们可以把Mn个结点的博弈树裁剪成M1/2n个结点，这是多么让人兴奋，也就是说625万个结点裁剪后只有2500个，原来10秒完成的事，现在不到1秒就可以完成，关键还在于α-β裁剪所需要的代码量少得惊人，只需要4行代码就可以让性能提升很多，具体的实现部分可以看我代码中的findCompMove和findHuamMove部分的注释（findCompMove中alpha是上一步的求值，beta为下一步的求值，在findHumanMove中则正好相反），而这个裁剪为什么成立在这里我不想赘述，具体可以看看这位博主的博客，图文并茂写得非常清晰
https://blog.csdn.net/potato_uncle/article/details/82861146

4.	启发式搜索
这一部分是在吸收了一些大佬们提供的方案后的改进部分，上面说过了我们通过α-β裁剪将指数部分减少了一半，但是在实际运行过程中我们发现底数其是远不止50尤其是一开始的时候，但是有很多位置非常明显是不合适落子的，如果将这些结点考虑进去会浪费非常多的时间，所以我们定义一个函数，用来搜索合适落子的位置，具体的方法是一个位置如果在两步之内有其他的落子（邻居）的话，这个位置就合适落子，而具体的实现办法在我后面的代码的inspireFind部分

5.	算杀
在完成前面的所用步骤后，一个新的问题出现了，四层深度的AI棋力并不怎么样，赢一个接触五子棋不久的新手问题不大，想要提升棋力主要手段是增加搜索深度，毕竟AI可以向后看得越多，它的棋力自然越高，但是正如前面看到的每加一层要花费的时间也成指数在增加，所以我们要另寻出路，如果AI在四层之后的搜索限定在必杀棋和对方不得不防的棋型上，那么这样的底数M应该非常小，就那么几个点，虽然这样会让AI在4层之后的判断上多少有一些偏差(对大局影响一般不大)，但是这成功的让AI向后多看了几步，棋力自然提升了不少，花费前四层全局搜索的时间算杀部分可以做到14到16层的查找，这样4+7的查找方法，足以让AI不通过深度学习，而战胜一般的玩家，算杀部分的代码与前面的全局搜索非常类似，具体代码部分可以查看我后面的compKillBoard和humanKillBoard函数

四．总结
  至此五子棋AI部分的关键部分已经全部讲述完了，在这个程序的设计过程中我们用到了非常多巧妙的数据结构，更是看到了四行代码提高指数倍速度的算法（实际上其中还包括非常多细节操作的算法，但是为了整体统一没有去介绍，在代码中的注释部分都有讲到），还有数不清的问题解决方法，这次的五子棋AI设计也让我自己领悟了不少东西

1.	任何时候都不要轻言放弃，也许再坚持会就成功了
我第一天就完成了程序流程图和绝多数代码的书写工作，第二天开始调试，那真的是最绝望的一天，上学时，自己很少调试较大的程序，缺少经验，大约1000行的程序代码其中的逻辑错误非常难以查找，大概2个小时左右解决了绝大多数明显的错误后，这时程序运行上AI还是经常出现一些很傻的棋招，多次查找都不知道哪里的问题，1个小时2个小时没有一点点进展，盯着屏幕看了整整三个小时，尝试过无数种方法，AI还是不仅出棋速度慢，出招还非常傻，后面的几个小时中我也多次想过放弃，可能自己能力还不够，无法完成，不过幸运的是最终还是坚持下来了，在1000行的代码中最终发现的问题是一个>写成<,一个COMPTER写成HUAMAN，就是这个让我改了几乎一天！

2.	谁都会说，但是谁都会做吗？实践胜于一切！
在一开始的程序算法考虑上，自认为对这些算法已经是非常了解了，但是在真正的编程过程中遇到的问题都是以前从来没有想到的问题，架构上的疏忽让自己在调试和编程中遇到了巨大的阻力，以前总是自我感觉良好，数据结构和算法单独实现都挺好，真正在实际的程序中的应用上问题频频。
第三天优化和重新架构程序，使得AI一步一步变得更加智能，并使得它的性能不断的提升，从一开始的1000多行，重新架构到600行，又优化到1000行，这一次不仅仅是AI性能的提升更是我自己的一种提升

花絮
最后说一下其他的吧，我的AI写完后，去看了下GitHub中大牛们写的非深度学习的五子棋AI程序，应该说厉害的人还是厉害，我的程序和他们比起来还是有一定的差距的，我找了一个和我的优化差不多的程序比较，棋力还是不相上下的，大牛用js写的，我的程序用的是c++,虽然他还用到了zobrist算法（一个棋类游戏中常用以优化AI的速度的算法，我嫌麻烦没有加），但是得益于c++语言的运行优势，我的AI在运行速度上只是慢了一点点
你们有兴趣可以去看看这位大牛的程序，用js写的可移植性很好，在手机网页上也可以打开，可以体验一下棋力
 链接：http://gobang.light7.cn/

截图
本来想发代码的，但是分离编译的代码不好发，我就截几张运行截图算了，后面会将其上传到GitHub的，感兴趣的可以去看看
 
     
                                   
                                                By: Y F 
                                                20